set quiet

#set aliases
self := justfile_directory()

[private]
default:
    just --list


create new_name:
    #!/usr/bin/env bash
    set -euo pipefail

    TEMPLATE_DIR="esp32-template"
    TARGET_DIR="{{ new_name }}"

    
    if [ ! -d "$TEMPLATE_DIR" ]; then
        echo "Error: Template directory '$TEMPLATE_DIR' does not exist." >&2
        exit 1
    fi

    if [ -e "$TARGET_DIR" ]; then
        echo "Error: Target '$TARGET_DIR' already exists." >&2
        exit 1
    fi

    echo "Copying $TEMPLATE_DIR to $TARGET_DIR"
    cp -r "$TEMPLATE_DIR" test
    echo "Done! Created $TARGET_DIR from $TEMPLATE_DIR."
