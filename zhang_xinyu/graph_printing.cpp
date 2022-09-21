
// graph1_2.cpp
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

class Graph{// vertex		edge
public:
    using VertexId = int; //頂點
    using EdgeId = int; //邊
    void add_vertex();
    void add_edge(VertexId from_vertex_id, VertexId to_vertex_id);    

	void print_all_vertex() {//打印全部顶点信息
		for (vertex_it = vertexes_.begin(); vertex_it < vertexes_.end(); ++vertex_it) {
			std::cout << (*vertex_it).id() << std::endl;//***********************
		}
	}	
	void print_all_edge() {//打印全部边信息
		for (edge_it = edges_.begin(); edge_it < edges_.end(); ++edge_it) {
			std::cout << (*edge_it).id() << " " << (*edge_it).from_vertex_id() << " " << (*edge_it).to_vertex_id() << std::endl;
		}
	}

	//************************************
	std::string print_json() const {
		//*******************************
		std::vector<Vertex>::const_iterator con_vertex_it;
		std::vector<Edge>::const_iterator con_edge_it;

		std::string str_json = "{\"vertices\":[";
		// 打印verticesa
		bool is_first_1 = true;
		for (con_vertex_it = vertexes_.begin(); con_vertex_it < vertexes_.end(); ++con_vertex_it) {
			if (is_first_1) {
				is_first_1 = false;
			}
			else {
				str_json += ",";
			}
			str_json += "{\"id\":" + std::to_string((*con_vertex_it).id()) + ",\"edge_ids\":[";
			bool is_first_2 = true;
			for (con_edge_it = edges_.begin(); con_edge_it < edges_.end(); ++con_edge_it) {
				if ((*con_edge_it).to_vertex_id() == ((*con_vertex_it).id()) || (*con_edge_it).from_vertex_id() == ((*con_vertex_it).id())) {
					if (is_first_2) {
						is_first_2 = false;
					}
					else {
						str_json += ",";
					}
					str_json += std::to_string((*con_edge_it).id());
					}
			}
		
			str_json += "]}";
		}
		// 打印 edges
		str_json += "],\"edges\":[";
		bool is_first_3 = true;
		
		for (con_edge_it = edges_.begin(); con_edge_it < edges_.end(); ++con_edge_it) {
			if (is_first_3) {
				is_first_3 = false;
			}
			else {
				str_json += ",";
			}
			str_json += "{\"id\":" + std::to_string((*con_edge_it).id()) + ",\"vertex_ids\":[" + std::to_string((*con_edge_it).from_vertex_id()) + "," + std::to_string((*con_edge_it).to_vertex_id()) + "]}";
		}

		return str_json += "]}";
	}

private:
    
    struct Vertex{// vertex *** 顶点
    public:
        explicit Vertex(VertexId id) : id_(id) {}
        VertexId id() const { return id_; }
        /*
	VertexId get_new_vertex_id(){
            return last_vertex_id_++;
        }
	*/
    private:
        VertexId id_ = 0;
        size_t last_vertex_id_ =0;
    };

    struct Edge{// edge *** 边
    public:
      	Edge(EdgeId id, VertexId from_vertex_id, VertexId to_vertex_id) :
        id_(id), from_vertex_id_(from_vertex_id), to_vertex_id_(to_vertex_id) {}

        EdgeId id() const { return id_; }
        VertexId from_vertex_id() const { return from_vertex_id_; }
        VertexId to_vertex_id() const { return to_vertex_id_; }
	
    private:
      	EdgeId id_ = 0;
      	VertexId from_vertex_id_ = 0;
      	VertexId to_vertex_id_ = 0;
    };
private://(class Graph)
    	std::vector<Vertex> vertexes_;
    	std::vector<Edge> edges_;
    	
    	std::vector<Vertex>::iterator vertex_it;//迭代器   vertex
    	std::vector<Edge>::iterator edge_it;    //迭代器   edge


    int last_vertex_id_ = 0;
    int last_edge_id_ = 0;
    friend int get_new_vertex_id(Graph & gr);
    friend int get_new_edge_id(Graph & gr);
};
//**************************************

namespace printing {
namespace json {
	
	std::string print_graph(const Graph & graph) {
		/*
		std::string str = "{\"vertices\":[";
		std::vector<Graph::Vertex>::iterator vertex_it;
		std::vector<Edge>::iterator edge_it;
		*/
		return graph.print_json();
	}
}
}

//**************************************

int get_new_vertex_id(Graph & gr) {
	return gr.last_vertex_id_++;
}

int get_new_edge_id(Graph & gr) {
	return gr.last_edge_id_++;
}

void Graph::add_vertex(){
    vertexes_.emplace_back(get_new_vertex_id(*this));
}

void Graph::add_edge(VertexId from_vertex_id, VertexId to_vertex_id){
    edges_.emplace_back(get_new_edge_id(*this), from_vertex_id, to_vertex_id);
}

void write_to_file(const std::string & text, const std::string & filename) {
	std::ofstream fout(filename);
	fout << text;
	fout.close();
}

int kVerticesCount = 13;

int main() {
  auto graph = Graph();

  for (int i = 0; i <= kVerticesCount; i++) {
    graph.add_vertex();
  }

  graph.add_edge(0, 1);
  graph.add_edge(0, 2);
  graph.add_edge(0, 3);
  graph.add_edge(1, 4);
  graph.add_edge(1, 5);
  graph.add_edge(1, 6);
  graph.add_edge(2, 7);
  graph.add_edge(2, 8);
  graph.add_edge(3, 9);
  graph.add_edge(4, 10);
  graph.add_edge(5, 10);
  graph.add_edge(6, 10);
  graph.add_edge(7, 11);
  graph.add_edge(8, 11);
  graph.add_edge(9, 12);
  graph.add_edge(10, 13);
  graph.add_edge(11, 13);
  graph.add_edge(12, 13);

  	/*
  	graph.print_all_vertex();//打印全部顶点的顶点id
	std::cout << std::endl;
	graph.print_all_edge();  //打印全部边的信息

	std::cout << graph.print_json() << std::endl;
	*/

//*****************************************************************************
//（在这里添加）
	const auto graph_json = printing::json::print_graph(graph);
	std::cout << graph_json << std::endl;
	write_to_file(graph_json, "graph.json");
//*****************************************************************************

  return 0;
}












