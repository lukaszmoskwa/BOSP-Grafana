CXX=g++
CPPFLAGS=-std=c++11
LDFLAGS=-I../barbeque/include -I../barbeque/include/bbque
LDLIBS=-L/usr/lib /usr/lib/libbbque_dci.so.1.0.0
INCLUDE=message_parser/resource_parser.cpp message_parser/app_parser.cpp

compile:
	$(CXX) grafana_client.cc $(INCLUDE) -o grafana_client $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -lboost_system 
