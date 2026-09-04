#!/bin/bash
set -e
cd "$(dirname "$0")"
python3 configure.py --bootstrap
cp ninja executable
