src = $(wildcard ./src/*.cpp)

obj = $(patsubst ./src/%.cpp, ./obj/%.o, $(src))

BIN = bin

target = main

inc_path = ./inc

all:$(target)			

$(target):$(obj)
	g++ -o $@ $^ -lpthread
	#@rm -f $(obj)
	@cp $@ ${BIN}
	@echo "编译结束"
 
$(obj):./obj/%.o : ./src/%.cpp
	g++ -c $< -o $@ -I $(inc_path)
 

clean:
	rm  -rf $(obj) $(target)



