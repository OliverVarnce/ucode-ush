echo "===================================="
echo "STDIN"
echo "------------------------------------"
echo "ls -la" | ./ush
echo "===================================="
echo "\n\n\n"
echo "===================================="
echo "New processes"
echo "------------------------------------"
./ush
ucode
echo "===================================="
echo "\n\n\n"
echo "===================================="
echo "New processes"
echo "------------------------------------"
/bin/ls
echo "\n\n\n"
echo "===================================="
echo "New processes"
echo "------------------------------------"
/bin/ls -laF
echo "\n\n\n"
echo "===================================="
echo "EXIT"
echo "------------------------------------"
exit 11
echo $?
echo "===================================="
echo "\n\n\n"
