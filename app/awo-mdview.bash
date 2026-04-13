# bash-completion code for the (awo) command 'mdview'

complete_awo_mdview()
{
    local current_word="${COMP_WORDS[COMP_CWORD]}"
    local pattern="${current_word}*.md"

    COMPREPLY=($(compgen -G "${pattern}"))
}

complete -F complete_awo_mdview mdview
