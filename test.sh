echo "-l test"
make
rm -f out1 out2 diff1
touch out1 out2
ls -l > out1
./ft_ls -l > out2
diff out1 out2
echo "Done"
