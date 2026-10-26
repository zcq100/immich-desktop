#!/usr/bin/env bash
# Sync `main` from upstream and trigger a build on the `ci` branch.
#
# `main` is a mirror of the upstream repository, so it carries no local CI of
# its own. This script (living on `ci`) fast-forwards origin/main to the latest
# upstream commit and then starts the `Build main (Linux amd64)` workflow.
#
# Usage: ./tools/sync-upstream.sh
#
# Requirements: git with push access to `origin`; either an authenticated `gh`
# CLI (preferred) or nothing else — the fallback pushes an empty commit to `ci`.

set -euo pipefail

UPSTREAM_REPO="${UPSTREAM_REPO:-https://github.com/hdmain/immich-desktop.git}"
UPSTREAM_REMOTE="${UPSTREAM_REMOTE:-upstream}"
BRANCH="${BRANCH:-main}"
CI_BRANCH="${CI_BRANCH:-ci}"
WORKFLOW="${WORKFLOW:-ci-linux.yml}"

die() {
    echo "error: $*" >&2
    exit 1
}

repo_root="$(git rev-parse --show-toplevel 2>/dev/null)" || die "not inside a git repository"
cd "$repo_root"

if ! git remote get-url origin >/dev/null 2>&1; then
    die "remote 'origin' is not configured"
fi

if ! git remote get-url "$UPSTREAM_REMOTE" >/dev/null 2>&1; then
    echo "Adding remote '$UPSTREAM_REMOTE' -> $UPSTREAM_REPO"
    git remote add "$UPSTREAM_REMOTE" "$UPSTREAM_REPO"
fi

echo "Fetching $UPSTREAM_REMOTE..."
git fetch "$UPSTREAM_REMOTE" --prune

upstream_sha="$(git rev-parse "$UPSTREAM_REMOTE/$BRANCH")"
upstream_short="${upstream_sha:0:7}"
echo "Upstream $BRANCH is at $upstream_sha"

# Mirror the upstream commit onto origin/main without touching the working tree.
echo "Updating origin/$BRANCH..."
git push origin "refs/remotes/$UPSTREAM_REMOTE/$BRANCH:refs/heads/$BRANCH"
git fetch origin --prune

# Keep a checked-out main in sync when it can fast-forward cleanly.
if [ "$(git rev-parse --abbrev-ref HEAD)" = "$BRANCH" ]; then
    if git merge --ff-only "origin/$BRANCH" >/dev/null 2>&1; then
        echo "Fast-forwarded local $BRANCH to $upstream_short"
    else
        echo "warning: local $BRANCH did not fast-forward; leaving it untouched" >&2
    fi
fi

trigger_with_gh() {
    command -v gh >/dev/null 2>&1 || return 1
    gh auth status >/dev/null 2>&1 || return 1
    gh workflow run "$WORKFLOW" --ref "$CI_BRANCH"
    sleep 3
    gh run list --workflow "$WORKFLOW" --branch "$CI_BRANCH" --limit 1 \
        --json url --jq '.[0].url' 2>/dev/null || true
}

trigger_with_commit() {
    echo "gh CLI unavailable or not authenticated; pushing an empty commit to $CI_BRANCH"
    git fetch origin "$CI_BRANCH"
    parent="$(git rev-parse FETCH_HEAD)"
    tree="$(git rev-parse "$parent^{tree}")"
    new_commit="$(git commit-tree "$tree" -p "$parent" \
        -m "ci: build $BRANCH@$upstream_short")"
    git push origin "$new_commit:refs/heads/$CI_BRANCH"
}

echo "Triggering $WORKFLOW on $CI_BRANCH..."
if ! trigger_with_gh; then
    trigger_with_commit
fi

echo "Done: building $BRANCH@$upstream_short"
