#!/usr/bin/env bash

set -Eeuxo pipefail
SCRIPT_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
BD3_ROOT="$(realpath ${SCRIPT_ROOT}/../../)"

(${BD3_ROOT}/dev-support/bin/lint_bazel.sh)
(${BD3_ROOT}/dev-support/bin/lint_cpp.sh)
