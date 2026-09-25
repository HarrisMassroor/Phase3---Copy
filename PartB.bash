#!/usr/bin/env bash

set -u

script_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)

if [[ $# -ne 1 ]]; then
	printf 'Usage: %s partA1|partA2|partA3|partA4\n' "$0" >&2
	exit 1
fi

version=$1
case "$version" in
	partA1|partA2|partA3|partA4) ;;
	*)
		printf 'Usage: %s partA1|partA2|partA3|partA4\n' "$0" >&2
		exit 1
		;;
esac

system=$(uname -s 2>/dev/null || printf 'unknown')
is_windows=false
case "$system" in
	MINGW*|MSYS*|CYGWIN*) is_windows=true ;;
esac

if [[ ${OS:-} == Windows_NT ]]; then
	is_windows=true
fi

if [[ $version == partA1 ]]; then
	if [[ $is_windows != true ]]; then
		printf '%s is only available on Windows.\n' "$version" >&2
		exit 1
	fi
else
	if [[ $system != Linux* ]]; then
		printf '%s is only available on Linux.\n' "$version" >&2
		exit 1
	fi
fi

program=''
case "$version" in
	partA1) candidates=("$script_dir/partA1.exe" "$script_dir/partA1"
		"$script_dir/A1.exe" "$script_dir/A1") ;;
	partA2) candidates=("$script_dir/partA2" "$script_dir/A2") ;;
	partA3) candidates=("$script_dir/partA3" "$script_dir/A3") ;;
	partA4) candidates=("$script_dir/partA4" "$script_dir/A4") ;;
esac

for candidate in "${candidates[@]}"; do
	if [[ -x $candidate ]]; then
		program=$candidate
		break
	fi
done

if [[ -z $program ]]; then
	printf 'Executable for %s was not found.\n' "$version" >&2
	exit 1
fi

while IFS= read -r line || [[ -n $line ]]; do
	read -r -a args <<< "$line"
	if [[ ${#args[@]} -ne 3 ||
		! ${args[0]:-} =~ ^[0-9]+$ ||
		! ${args[1]:-} =~ ^[0-9]+$ ||
		! ${args[2]:-} =~ ^[0-9]+$ ||
		${args[0]:-0} =~ ^0+$ ||
		${args[1]:-0} =~ ^0+$ ||
		${args[2]:-0} =~ ^0+$ ]]; then
		printf 'Invalid input (expected three positive integers): %s\n' "$line" >&2
		continue
	fi

	"$program" "${args[0]}" "${args[1]}" "${args[2]}"
done