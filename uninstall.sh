
installf='install_manifest.txt'

if test -f "$installf"
then

  printf "Removing:\n"
  ! xargs rm -v  < "$installf" && exit
  rm -v "$installf"

fi

