#!/bin/bash -eux

(
  cd puara-arduino || exit 1

  git config user.name "puara-bot"
  git config user.email "puara-bot@sat.qc.ca"

  git checkout -b continuous

  git commit -am "$GITHUB_REF :: $GITHUB_SHA"

  git push --set-upstream origin continuous --force
)
