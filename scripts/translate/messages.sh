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

EXTOPTS=(
    "--c++"
    "--kde"
    "--from-code=UTF-8"
    "-ci18n"
    "-ki18n:1" "-ki18nc:1c,2" "-ki18np:1,2" "-ki18ncp:1c,2,3"
    "-kki18n:1" "-kki18nc:1c,2" "-kki18np:1,2" "-kki18ncp:1c,2,3"
    "-kxi18n:1" "-kxi18nc:1c,2" "-kxi18np:1,2" "-kxi18ncp:1c,2,3"
    "-kkxi18n:1" "-kkxi18nc:1c,2" "-kkxi18np:1,2" "-kkxi18ncp:1c,2,3"
    "-kI18N_NOOP:1" "-kI18NC_NOOP:1c,2"
    "-kI18N_NOOP2:1c,2" "-kI18N_NOOP2_NOSTRIP:1c,2"
    "-ktr2i18n:1" "-ktr2xi18n:1"
    "-kN_:1"
    "-kaliasLocale"
    "--width=200"
    "--add-location=file"
    "--package-name=$PROJECT_NAME"
    "--package-version=$PROJECT_VERSION"
    "--msgid-bugs-address=$BUG_ADDRESS"
)

echoGray "[translate/messages] Extracting messages."
(find "$PROJECT_DIR/src" -name '*.cpp' -o -name '*.hpp' -o -name '*.hpp.in' -o -name '*.cpp.in' | sort | xargs xgettext "${EXTOPTS[@]}" -o "$POT_FILE.new") || { echoRed "[translate/messages] Error while calling xgettext. Aborting."; exit 1; }

sed -i 's/# SOME DESCRIPTIVE TITLE./'"# Translation of $PROJECT_NAME in LANGUAGE"'/' "$POT_FILE.new"
sed -i 's/# Copyright (C) YEAR THE PACKAGE'"'"'S COPYRIGHT HOLDER/'"# Copyright (C) $(date +%Y)"'/' "$POT_FILE.new"
sed -i 's;'"$PROJECT_DIR"';\.\./\.\./\.\.;' "$POT_FILE.new"

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
# TODO: extract .desktop

#---
# Populate lang/progress.md
echoGray "[translate/messages] Updating lang/progress.md"
# sed -i -E 's`share\/plasma\/plasmoids\/(.+)\/translate`share/plasma/plasmoids/'"$PROJECT_NAME"'/translate`' "$PROGRESS_FILE"
sed -i '/^|/ d' "$PROGRESS_FILE" # Remove status table from Progress
cat "$STATUS_FILE" >> "$PROGRESS_FILE"
rm "$STATUS_FILE"

echoGreen "[translate/messages] Done merge script"
