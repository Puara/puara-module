#!/bin/bash -eux

PUARA_MODULE_SOURCES=(
  src data
  .clang-format .gitignore library.properties
  LICENSE README.md
)

(
  cd puara-arduino || exit 1
  git add "${PUARA_MODULE_SOURCES[@]}" examples
  git commit -m "$GITHUB_REF :: $GITHUB_SHA"

  git push --set-upstream origin continuous --force
)
