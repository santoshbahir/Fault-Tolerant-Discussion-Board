make clean #clean current directory

cp *.* makefile server2/
cp *.* makefile server3/
cp *.* makefile server4/
cp *.* makefile server5/

echo ""
echo "Compiling 1 th server"
make

echo ""
echo "Compiling 2 th server"
cd server2/ #compile second server
make clean
make

echo ""
echo "Compiling 3 th server"
cd ../server3/ #compile third server
make clean
make

echo ""
echo "Compiling 4 th server"
cd ../server4/ #compile fourth server
make clean
make

echo ""
echo "Compiling 5 th server"
cd ../server5/ #compile fifth server
make clean
make
