echo "One-One Testing Start";
gcc one-one/test_files/testcode1.c one-one/queue.c one-one/lock.c one-one/my_thread.c -o testcode1
./testcode1
rm -r testcode1

gcc one-one/test_files/testcode2.c one-one/queue.c one-one/lock.c one-one/my_thread.c -o testcode2
./testcode2
rm -r testcode2

gcc one-one/test_files/testcode3.c one-one/queue.c one-one/lock.c one-one/my_thread.c -o testcode3
./testcode3
rm -r testcode3

gcc one-one/test_files/testcode4.c one-one/queue.c one-one/lock.c one-one/my_thread.c -o testcode4
./testcode4
rm -r testcode4

gcc one-one/test_files/testcode5.c one-one/queue.c one-one/lock.c one-one/my_thread.c -o testcode5
./testcode5
rm -r testcode5

gcc one-one/test_files/testcode6.c one-one/queue.c one-one/lock.c one-one/my_thread.c -o testcode6
./testcode6
rm -r testcode6

gcc one-one/test_files/testcode7.c one-one/queue.c one-one/lock.c one-one/my_thread.c -o testcode7
./testcode7
rm -r testcode7

gcc one-one/test_files/testcode8.c one-one/queue.c one-one/lock.c one-one/my_thread.c -o testcode8
./testcode8
rm -r testcode8
