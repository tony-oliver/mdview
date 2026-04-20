# bash-completion code for the command 'mdview'.
#
# Copyright © 2026, Tony Oliver.  All rights reserved.
#
# This file should be installed in /etc/bash_completion.d,
# from where its can be sourced by each instance of bash.

_mdview_bash_completion_()
{
    local current_word="${COMP_WORDS[COMP_CWORD]}"
    local file_pattern="${current_word}*.md"

    compgen -V COMPREPLY -G "${file_pattern}" -o dirnames
}

complete -F _mdview_bash_completion_ mdview
