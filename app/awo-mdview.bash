# bash-completion code for the (awo) command 'mdview'

complete_awo_mdview()
{
    COMPREPLY=( $(compgen -G "${COMP_WORDS[COMP_CWORD]}*.md") )
}

complete -F complete_awo_mdview mdview
