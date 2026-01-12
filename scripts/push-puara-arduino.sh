#!/bin/bash -eux
PUARA_MODULE_SOURCES=(
  src data
  .clang-format .gitignore library.properties
  LICENSE README.md
)

(
  cd puara-arduino || exit 1

  git config user.name "puara-bot"
  git config user.email "puara-bot@sat.qc.ca"

  git checkout -b continuous

  git pull

  git add "${PUARA_MODULE_SOURCES}" examples --ignore-errors
  git commit -am "$GITHUB_REF :: $GITHUB_SHA"

  # looks to see if there is something to commit.
  if [ `git status --porcelain=1 | wc -l` -eq 0 ]; then
      echo "Nothing to commit."
      exit 0
  else
      git push --set-upstream origin continuous --force
  fi

)
