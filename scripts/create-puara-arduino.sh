#!/bin/bash -eux

PUARA_MODULE_SOURCES=(
  src data
  .clang-format .gitignore library.properties
  LICENSE README.md
)

PUARA_TEMPLATES=(
  basic basic-gestures basic-osc ble-advertising button-osc libmapper-osc
)

# 1. Recreate repo

(
  cd puara-arduino || exit 1

  git config user.name "puara-bot"
  git config user.email "puara-bot@sat.qc.ca"

  git checkout -b continuous
  rm -rf "${PUARA_MODULE_SOURCES[@]}" examples
)

# 2. Copy puara-module
(
  cd puara-module || exit 1
  cp -rf "${PUARA_MODULE_SOURCES[@]}" ../puara-arduino/
)

# 3. Copy puara-module-templates
(
  cd puara-module-templates || exit 1

  shopt -s nullglob

  for template in "${PUARA_TEMPLATES[@]}"; do
    mkdir -p "../puara-arduino/examples/$template"
    cp -rf "$template/src/main.cpp" "../puara-arduino/examples/$template/$template.ino"
    # remove main.cpp to avoid copying it again.
    rm "$template/src/main.cpp"
    headers=( "$template/src/"*.{c,cpp,h,hpp} )
    if ((${#headers[@]})); then
      cp -rf "${headers[@]}" "../puara-arduino/examples/$template/"
    fi

    cp -rf "$template/data" "../puara-arduino/examples/$template/data"
  done
)
