# SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: GPL-3.0-only

#!/usr/bin/env bash

SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
SCRIPT_DIR="$(readlink -f "$SCRIPT_DIR")"

PROJECT_DIR="$(readlink -f "$SCRIPT_DIR/../..")"
LANG_DIR="$PROJECT_DIR/lang"
POT_FILE="$LANG_DIR/mease.pot"
PO_DIR="$LANG_DIR/po"
STATUS_FILE="$LANG_DIR/status.md"
PROGRESS_FILE="$LANG_DIR/progress.md"

### Colors
# https://stackoverflow.com/questions/5947742/how-to-change-the-output-color-of-echo-in-linux
# https://stackoverflow.com/questions/911168/how-can-i-detect-if-my-shell-script-is-running-through-a-pipe
TC_Red='\033[31m'
TC_Orange='\033[33m'
TC_LightGray='\033[90m'
TC_LightRed='\033[91m'
TC_LightGreen='\033[92m'
TC_Yellow='\033[93m'
TC_LightBlue='\033[94m'
TC_Reset='\033[0m'
TC_Bold='\033[1m'

if [ ! -t 1 ]; then
    TC_Red=''
    TC_Orange=''
    TC_LightGray=''
    TC_LightRed=''
    TC_LightGreen=''
    TC_Yellow=''
    TC_LightBlue=''
    TC_Bold=''
    TC_Reset=''
fi

function echoTC() {
    text="$1"
    textColor="$2"
    echo -e "${textColor}${text}${TC_Reset}"
}

function echoGray {
    echoTC "$1" "$TC_LightGray"
}

function echoRed {
    echoTC "$1" "$TC_Red"
}

function echoGreen {
    echoTC "$1" "$TC_LightGreen"
}

### Show Args
function show_help() {
    echo "Usage: message.sh [OPTIONS]"
    echo "Extract translatable text"
    echo ""
    echo "Options:"
    echo "    -h, --help              show this help message"
    echo "    -n, --project-name      project name"
    echo "    -b, --bug-report        bug report address"
    echo "    -v, --project-version   project version"
}

while [[ $# -gt 0 ]]; do
    case $1 in
        -n|--project-name)
            PROJECT_NAME="$2"
            shift
            shift
            ;;
        -v|--project-version)
            PROJECT_VERSION="$2"
            shift
            shift
            ;;
        -b|--bug-report)
            BUG_ADDRESS="$2"
            shift
            shift
            ;;
        -h|--help)
            show_help
            shift
            exit 0
            ;;
        *)
            shift
            ;;
    esac
done

if [[ -z "$PROJECT_NAME" ]]; then
    echoRed "[translate/messages] Error: Missing project name."
    exit 1
fi

if [[ -z "$PROJECT_VERSION" ]]; then
    echoRed "[translate/messages] Error: Missing project version."
    exit 1
fi

if [[ -z "$BUG_ADDRESS" ]]; then
    echoRed "[translate/messages] Error: Missing project bug report address."
    exit 1
fi

echoGray "[translate/messages] Extracting messages."

find "$PROJECT_DIR" \( -name '*.desktop' -o -name '*.desktop.in' \) -not -path "$PROJECT_DIR/build/*" | sort > "$SCRIPT_DIR/infiles.list"
xgettext \
    --files-from="$SCRIPT_DIR/infiles.list" \
    --from-code=UTF-8 \
    --width=200 \
    --add-location=file \
    --language=Desktop \
    -k -kName -kGenericName -kComment -kKeywords \
    -D "$PROJECT_DIR" \
    -o "$POT_FILE.new" \
|| { echoRed "[translate/messages] Error while calling xgettext. Aborting."; exit 1; }

find "$PROJECT_DIR/mimetype" -name '*.xml' | sort > "$SCRIPT_DIR/infiles.list"
xgettext \
    --files-from="$SCRIPT_DIR/infiles.list" \
    --from-code=UTF-8 \
    --width=200 \
    --add-location=file \
    --join-existing \
    -D "$PROJECT_DIR" \
    -o "$POT_FILE.new" \
|| { echoRed "[translate/messages] Error while calling xgettext. Aborting."; exit 1; }

find "$PROJECT_DIR" \( -name '*.cpp' -o -name '*.hpp' -o -name '*.hpp.in' -o -name '*.cpp.in' \) -not -path "$PROJECT_DIR/build/*" | sort > "$SCRIPT_DIR/infiles.list"
xgettext \
    --files-from="$SCRIPT_DIR/infiles.list" \
    --c++ \
    --kde \
    --from-code=UTF-8 \
    -ci18n \
    -ki18n:1 -ki18nc:1c,2 -ki18np:1,2 -ki18ncp:1c,2,3 \
    -kki18n:1 -kki18nc:1c,2 -kki18np:1,2 -kki18ncp:1c,2,3 \
    -kxi18n:1 -kxi18nc:1c,2 -kxi18np:1,2 -kxi18ncp:1c,2,3 \
    -kkxi18n:1 -kkxi18nc:1c,2 -kkxi18np:1,2 -kkxi18ncp:1c,2,3 \
    -kI18N_NOOP:1 -kI18NC_NOOP:1c,2 \
    -kI18N_NOOP2:1c,2 -kI18N_NOOP2_NOSTRIP:1c,2 \
    -ktr2i18n:1 -ktr2xi18n:1 \
    -kN_:1 \
    -kaliasLocale \
    --width=200 \
    --add-location=file \
    --package-name="$PROJECT_NAME" \
    --package-version="$PROJECT_VERSION" \
    --msgid-bugs-address="$BUG_ADDRESS" \
    --join-existing \
    -D "$PROJECT_DIR" \
    -o "$POT_FILE.new" \
    || { echoRed "[translate/messages] Error while calling xgettext. Aborting."; exit 1; }

rm "$SCRIPT_DIR/infiles.list"

sed -i 's/# SOME DESCRIPTIVE TITLE./'"# Translation of $PROJECT_NAME in LANGUAGE"'/' "$POT_FILE.new"
sed -i 's/# Copyright (C) YEAR THE PACKAGE'"'"'S COPYRIGHT HOLDER/'"# Copyright (C) $(date +%Y)"'/' "$POT_FILE.new"
sed -i 's|'"$PROJECT_DIR"'|\.\./\.\./\.\.|g' "$POT_FILE.new"

if [[ -f "$POT_FILE" ]]; then
    newPotDate=$(grep "POT-Creation-Date:" "$POT_FILE.new" | sed 's/.\{3\}$//')
    oldPotDate=$(grep "POT-Creation-Date:" "$POT_FILE" | sed 's/.\{3\}$//')

    sed -i 's/'"${newPotDate}"'/'"${oldPotDate}"'/' "$POT_FILE.new"

    changes=$(diff "$POT_FILE" "$POT_FILE.new")

    if [[ -n "$changes" ]]; then
        # There's been changes
        sed -i 's/'"${oldPotDate}"'/'"${newPotDate}"'/' "$POT_FILE.new"
        mv "$POT_FILE.new" "$POT_FILE"

        addedKeys=$(echo "$changes" | grep "> msgid" | cut -c 9- | sort)
        removedKeys=$(echo "$changes" | grep "< msgid" | cut -c 9- | sort)

        if [[ -n "$addedKeys" ]]; then
            echoGreen "Added Keys:"
            echoGreen "$addedKeys"
        fi

        if [[ -n "$removedKeys" ]]; then
            echoRed "Removed Keys:"
            echoRed "$removedKeys"
        fi
    else
        # No changes
        rm "$POT_FILE.new"
    fi
else
    # template.pot didn't already exist
    mv "$POT_FILE.new" "$POT_FILE"
fi

potMessageCount=$(grep -Pzo 'msgstr ""\n(\n|$)' "$POT_FILE" | grep -c 'msgstr ""')
echo "|  Locale  |  Lines  | % Done|" > "$STATUS_FILE"
echo "|----------|---------|-------|" >> "$STATUS_FILE"
entryFormat="| %-8s | %7s | %5s |"
templateLine=$(perl -e "printf(\"$entryFormat\", \"Template\", \"${potMessageCount}\", \"\")")
echo "$templateLine" >> "$STATUS_FILE"

echoGray "[translate/messages] Done extracting messages."

#---
echoGray "[translate/messages] Merging messages."
mapfile -t catalogs < <(find "$PO_DIR" -name '*.po' | sort)
for cat in "${catalogs[@]}"; do
    echoGray "[translate/messages] Updating ${cat}"
    catLocale=$(basename -- "$(dirname -- "$cat")")

    widthArg=""
    catUsesGenerator=$(grep "X-Generator:" "$cat")

    if [[ -z "$catUsesGenerator" ]]; then
        widthArg="--width=400"
    fi

    compendiumArg=()
    if [[ -n "$COMPENDIUM_DIR" ]]; then
        langCode=$(basename -- "${cat%.*}")
        compendiumPath=$(realpath "$COMPENDIUM_DIR/compendium-${langCode}.po")
        if [ -f "$compendiumPath" ]; then
            echo "compendiumPath=$compendiumPath"
            compendiumArg+=("--compendium=$compendiumPath")
        fi
    fi

    cp "$cat" "$cat.new"
    sed -i 's/"Content-Type: text\/plain; charset=CHARSET\\n"/"Content-Type: text\/plain; charset=UTF-8\\n"/' "$cat.new"

    msgmerge \
        ${widthArg} \
        --add-location=file \
        --no-fuzzy-matching \
        "${compendiumArg[@]}" \
        -o "$cat.new" \
        "$cat.new" "$POT_FILE"

    sed -i 's/# SOME DESCRIPTIVE TITLE./'"# Translation of $PROJECT_NAME in ${catLocale}"'/' "$cat.new"
    sed -i 's/# Translation of '"$PROJECT_NAME"' in LANGUAGE/'"# Translation of $PROJECT_NAME in ${catLocale}"'/' "$cat.new"
    sed -i 's/# Copyright (C) YEAR THE PACKAGE'"'"'S COPYRIGHT HOLDER/'"# Copyright (C) $(date +%Y)"'/' "$cat.new"

    poEmptyMessageCount=$(grep -Pzo 'msgstr ""\n(\n|$)' "$cat.new" | grep -c 'msgstr ""')
    poMessagesDoneCount=$(("$potMessageCount" - "$poEmptyMessageCount"))
    poCompletion=$(perl -e "printf(\"%d\", $poMessagesDoneCount * 100 / $potMessageCount)")
    poLine=$(perl -e "printf(\"$entryFormat\", \"$catLocale\", \"${poMessagesDoneCount}/${potMessageCount}\", \"${poCompletion}%\")")
    echo "$poLine" >> "$STATUS_FILE"

    mv "$cat.new" "$cat"
done
echoGray "[translate/messages] Done merging messages"

#---
# Generate LINGUAS for msgfmt
if [ -f "$SCRIPT_DIR/po/LINGUAS" ]; then
    rm "$SCRIPT_DIR/po/LINGUAS"
fi
mkdir -p "$SCRIPT_DIR/po"
touch "$SCRIPT_DIR/po/LINGUAS"
for cat in "${catalogs[@]}"; do
    catLocale=$(basename -- "$(dirname -- "$cat")")
    cp "$LANG_DIR/po/$catLocale/mease.po" "$SCRIPT_DIR/po/$catLocale.po"
    echo "${catLocale}" >> "$SCRIPT_DIR/po/LINGUAS"
done

echoGray "[translate/messages] Updating .desktop file"

cp -f "$PROJECT_DIR/mease.desktop.in" "$SCRIPT_DIR/template.desktop"
sed -i '/^Name\[/ d; /^GenericName\[/ d; /^Comment\[/ d; /^Keywords\[/ d' "$SCRIPT_DIR/template.desktop"

msgfmt \
    --desktop \
    --template="$SCRIPT_DIR/template.desktop" \
    --verbose \
    -d "$SCRIPT_DIR/po" \
    -o "$SCRIPT_DIR/new.desktop"

# Delete empty msgid messages that used the po header
if grep -q '^Name=$' "$SCRIPT_DIR/new.desktop"; then
    echo "[translate/merge] Name in mease.desktop.in is empty!"
    sed -i '/^Name\[/ d' "$SCRIPT_DIR/new.desktop"
fi
if grep -q '^GenericName=$' "$SCRIPT_DIR/new.desktop"; then
    echo "[translate/merge] GenericName in mease.desktop.in is empty!"
    sed -i '/^GenericName\[/ d' "$SCRIPT_DIR/new.desktop"
fi
if grep -q '^Comment=$' "$SCRIPT_DIR/new.desktop"; then
    echo "[translate/merge] Comment in mease.desktop.in is empty!"
    sed -i '/^Comment\[/ d' "$SCRIPT_DIR/new.desktop"
fi
if grep -q '^Keywords=$' "$SCRIPT_DIR/new.desktop"; then
    echo "[translate/merge] Keywords in mease.desktop.in is empty!"
    sed -i '/^Keywords\[/ d' "$SCRIPT_DIR/new.desktop"
fi

# Place translations at the bottom of the desktop file.
translatedLines=$(grep "]=" < "$SCRIPT_DIR/new.desktop")
if [[ -n "${translatedLines}" ]]; then
    sed -i '/^Name\[/ d; /^GenericName\[/ d; /^Comment\[/ d; /^Keywords\[/ d' "$SCRIPT_DIR/new.desktop"
    if [[ "$(tail -c 2 "$SCRIPT_DIR/new.desktop" | wc -l)" != "2" ]]; then
        # Does not end with 2 empty lines, so add an empty line.
        echo "" >> "$SCRIPT_DIR/new.desktop"
    fi
    echo "${translatedLines}" >> "$SCRIPT_DIR/new.desktop"
fi

mv "$SCRIPT_DIR/new.desktop" "$PROJECT_DIR/mease.desktop.in"
rm "$SCRIPT_DIR/template.desktop"

echoGray "[translate/messages] Updating mimetype file"

mapfile -t mimetype_files < <(find "$PROJECT_DIR/mimetype" -name '*.xml' | sort)

for mimetype_file in "${mimetype_files[@]}"; do
    cp -f "$mimetype_file" "$SCRIPT_DIR/template.xml"

    msgfmt \
        --xml \
        --template="$SCRIPT_DIR/template.xml" \
        --verbose \
        -d "$SCRIPT_DIR/po" \
        -o "$SCRIPT_DIR/new.xml"

    mv "$SCRIPT_DIR/new.xml" "$PROJECT_DIR/mimetype/$(realpath -s --relative-to="$PROJECT_DIR/mimetype" "$mimetype_file")"
    rm "$SCRIPT_DIR/template.xml"
done

# Cleanup
rm -r "$SCRIPT_DIR/po"

#---
# Populate lang/progress.md
echoGray "[translate/messages] Updating lang/progress.md"
# sed -i -E 's`share\/plasma\/plasmoids\/(.+)\/translate`share/plasma/plasmoids/'"$PROJECT_NAME"'/translate`' "$PROGRESS_FILE"
sed -i '/^|/ d' "$PROGRESS_FILE" # Remove status table from Progress
cat "$STATUS_FILE" >> "$PROGRESS_FILE"
rm "$STATUS_FILE"

echoGreen "[translate/messages] Done merge script"
