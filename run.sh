FLAGS="-Wall -std=c++17"
SRCS="main.cpp commands/commands.cpp commands/helper.cpp inventory/inventory.cpp product/product.cpp utils/utils.cpp"

g++ $FLAGS -o main $SRCS && ./main
