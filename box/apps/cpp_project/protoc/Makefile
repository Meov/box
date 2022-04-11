TARGET:=test
CXX_FLAG:=`pkg-config --cflags --libs protobuf`

$(TARGET) : main.cpp student.pb.cc
	g++ -o $@ $^ $(CXX_FLAG)

#get cpp class of student proto
student.pb.cc:student.proto
	protoc student.proto --cpp_out=./

clean:
	$(RM) -rf $(TARGET)

