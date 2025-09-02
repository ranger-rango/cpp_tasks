#!/usr/bin/env bash
#
# Run crypto_tool with local lib/ dependencies
#

# Resolve script directory (so it works from anywhere)
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Set library path to local lib
export LD_LIBRARY_PATH="$SCRIPT_DIR/lib:$LD_LIBRARY_PATH"

# Run the tool with forwarded arguments
"$SCRIPT_DIR/bin/crypto_tool" "$@"
