echo "Normal test"
rm -f out1 out2
touch out1 out2
ls -1 sandbox > out1
./ft_ls -1 sandbox > out2
diff out1 out2
echo "Done" 
