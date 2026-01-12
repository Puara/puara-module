#!/bin/bash -eux

(
  cd puara-arduino || exit 1

  git config user.name "puara-bot"
  git config user.email "puara-bot@sat.qc.ca"

  git add src/* data/* examples/* LICENSE README.md library.properties .clang-format .gitignore --ignore-errors
  git commit -am "$GITHUB_REF :: $GITHUB_SHA"

  # looks to see if there is something to commit.
  if [ `git status --porcelain=1 | wc -l` -eq 0 ]; then
      echo "Nothing to commit."
      exit 0
  else
      git push --set-upstream origin continuous --force
  fi

)
