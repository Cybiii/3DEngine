cd test_build && g++ -c ../Tests/Phase2BasicTests.cpp -I../Engine -DENGINE_DEBUG -std=c++17 -o Phase2BasicTests.o && g++ Phase2BasicTests.o Logger.o -o Phase2BasicTests.exe && Phase2BasicTests.exe
