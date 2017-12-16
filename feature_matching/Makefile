NVCC        = nvcc

NVCC_FLAGS  = -I/usr/local/cuda/include -gencode=arch=compute_50,code=\"sm_50,compute_50\"

ifdef dbg
	NVCC_FLAGS  += -g -G
else
	NVCC_FLAGS  += -O3
endif

OBJ_DIR 	= object_files
LD_FLAGS    = -lcudart -L/usr/local/cuda/lib64
EXE	        = feature_matching
OBJ	        = main.o utility.o gpu_matcher_cu.o
OPENCV_LIBS = `pkg-config opencv --cflags --libs`


default: $(EXE)

$(EXE): $(OBJ)
	@$(NVCC) $(addprefix $(OBJ_DIR)/, $(OBJ)) $(LD_FLAGS) $(NVCC_FLAGS) \
	$(OPENCV_LIBS) -o $(EXE)
	@echo "Build successful!"

main.o: src/main.cpp include/utility.hpp include/gpu_matcher.hpp \
	include/matrix.hpp
	@echo "Compiling code, hang on!"
	@mkdir -p $(OBJ_DIR)
	@$(NVCC) -c src/main.cpp $(NVCC_FLAGS) $(LIBS) -o $(OBJ_DIR)/$@

utility.o: src/utility.cpp include/utility.hpp include/matrix.hpp
	@$(NVCC) -c src/utility.cpp $(NVCC_FLAGS) $(LIBS) -o $(OBJ_DIR)/$@

gpu_matcher_cu.o: src/gpu_matcher.cu src/kernels.cu include/gpu_matcher.hpp \
	include/matrix.hpp
	@$(NVCC) -c src/gpu_matcher.cu $(NVCC_FLAGS) $(LIBS) -o $(OBJ_DIR)/$@


clean:
	@rm -rf $(OBJ_DIR)/*.o $(OBJ_DIR)/$(EXE)
	@rmdir --ignore-fail-on-non-empty $(OBJ_DIR)
	@echo "Cleaned build files!"
