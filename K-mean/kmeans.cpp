#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <time.h>
#include <algorithm>

using namespace std;

class Image{
private:
	int id_image, id_cluster;
	vector<double> values;
	int total_values;
	string name;

public:
	Image(int id_image, vector<double>& values, string name = ""){
		this->id_image = id_image;
		total_values = values.size();
		for(int i = 0; i < total_values; i++){
			this->values.push_back(values[i]);
		}
		this->name = name;
		id_cluster = -1;
	}
	int getID(){
		return id_image;
	}
	void setCluster(int id_cluster){
		this->id_cluster = id_cluster;
	}
	int getCluster(){
		return id_cluster;
	}
	double getValue(int index){
		return values[index];
	}
	int getTotalValues(){
		return total_values;
	}
	void addValue(double value){
		values.push_back(value);
	}
	string getName(){
		return name;
	}
};

class Cluster{
private:
	int id_cluster;
	vector<double> central_values;
	vector<Image> images;

public:
	Cluster(int id_cluster, Image image){
		this->id_cluster = id_cluster;
		int total_values = image.getTotalValues();
		for(int i = 0; i < total_values; i++){
			central_values.push_back(image.getValue(i));
		}
		images.push_back(image);
	}
	void addImage(Image image){
		images.push_back(image);
	}
	bool removeImage(int id_image){
		int total_images = images.size();
		for(int i = 0; i < total_images; i++){
			if(images[i].getID() == id_image){
				images.erase(images.begin() + i);
				return true;
			}
		}
		return false;
	}
	double getCentralValue(int index){
		return central_values[index];
	}
	void setCentralValue(int index, double value){
		central_values[index] = value;
	}
	Image getImage(int index){
		return images[index];
	}
	int getTotalImages(){
		return images.size();
	}
	int getID(){
		return id_cluster;
	}
};

class KMeans{
private:
	int K; // number of clusters
	int total_values, total_images, max_iterations;
	vector<Cluster> clusters;

	// return ID of nearest image (using euclidean distance)
	int getIDNearestCenter(Image image){
		double sum = 0.0, min_dist;
		int id_cluster_center = 0;
		
		for(int i = 0; i < total_values; i++){
			sum += pow(clusters[0].getCentralValue(i) -
					   image.getValue(i), 2.0);
		}
		min_dist = sqrt(sum);
		for(int i = 1; i < K; i++){
			double dist;
			sum = 0.0;
			for(int j = 0; j < total_values; j++){
				sum += pow(clusters[i].getCentralValue(j) -
						   image.getValue(j), 2.0);
			}
			dist = sqrt(sum);
			if(dist < min_dist){
				min_dist = dist;
				id_cluster_center = i;
			}
		}
		return id_cluster_center;
	}

public:
	KMeans(int K, int total_images, int total_values, int max_iterations){
		this->K = K;
		this->total_images = total_images;
		this->total_values = total_values;
		this->max_iterations = max_iterations;
	}

	void run(vector<Image> & images){
		if(K > total_images) return;
		vector<int> prohibited_indexes;
		// choose K distinct values for the centers of the clusters
		for(int i = 0; i < K; i++){
			while(true){
				int index_image = rand() % total_images;
				if(find(prohibited_indexes.begin(), prohibited_indexes.end(), index_image) == prohibited_indexes.end()){
					prohibited_indexes.push_back(index_image);
					images[index_image].setCluster(i);
					Cluster cluster(i, images[index_image]);
					clusters.push_back(cluster);
					break;
				}
			}
		}

		int iter = 1;
		while(true){
			bool done = true;
			// associates each image to the nearest center
			for(int i = 0; i < total_images; i++){
				int id_old_cluster = images[i].getCluster();
				int id_nearest_center = getIDNearestCenter(images[i]);
				
				if(id_old_cluster != id_nearest_center){
					if(id_old_cluster != -1)
						clusters[id_old_cluster].removeImage(images[i].getID());

					images[i].setCluster(id_nearest_center);
					clusters[id_nearest_center].addImage(images[i]);
					done = false;
				}
			}
			// recalculating the center of each cluster
			for(int i = 0; i < K; i++){
				for(int j = 0; j < total_values; j++){
					int total_images_cluster = clusters[i].getTotalImages();
					double sum = 0.0;
					if(total_images_cluster > 0){
						for(int p = 0; p < total_images_cluster; p++)
							sum += clusters[i].getImage(p).getValue(j);
						clusters[i].setCentralValue(j, sum / total_images_cluster);
					}
				}
			}
			if(done == true || iter >= max_iterations){
				cout << "Break in iteration " << iter << "\n\n";
				break;
			}
			iter++;
		}
		// shows elements of clusters
		for(int i = 0; i < K; i++){
			int total_images_cluster =  clusters[i].getTotalImages();
			cout << "Cluster " << clusters[i].getID() + 1 << endl;
			for(int j = 0; j < total_images_cluster; j++){
				cout << "Image " << clusters[i].getImage(j).getID() + 1 << ": ";
				for(int p = 0; p < total_values; p++){
					cout << clusters[i].getImage(j).getValue(p) << " ";
				}
				string image_name = clusters[i].getImage(j).getName();
				if(image_name != ""){
					cout << "- " << image_name;
				}
				cout << endl;
			}
			cout << "Cluster values: ";
			for(int j = 0; j < total_values; j++){
				cout << clusters[i].getCentralValue(j) << " ";
			}
			cout << "\n\n\n";
		}
	}
};

int main(int argc, char *argv[]){
	srand (time(NULL));
	int total_images, total_values, K, max_iterations, has_name;
	string filename;
	cout << "Enter a name of dataset file which is in the same folder of the code: ";
	getline( cin, filename );
	filename += ".txt";
	ifstream infile(filename.c_str());
	if (!infile.is_open()){    //checks to see if file opens properly
	    cerr << "Error: Could not find the requested file.";
	}else{
	    infile >> total_images >> total_values >> K >> max_iterations >> has_name;
	    cout << "\nTotal images: " << total_images << "\n";
	    cout << "Total_values: " <<  total_values << "\n";
		cout << "K number: " << K << "\n";
	}
	vector<Image> images;
	string image_name;
	int i=0;
	while(infile){
		vector<double> values;
		double vl;
		for(int j = 0; j < total_values; j++){
			infile >> vl;
			values.push_back(vl);
		}
		if(has_name){
			infile >> image_name;
			Image p(i, values, image_name);
			images.push_back(p);
		}
		else{
			Image p(i, values);
			images.push_back(p);
		}
		i++;
	}
	infile.close(); // Remember to close the file.
	KMeans kmeans(K, total_images, total_values, max_iterations);
	kmeans.run(images);
	return 0;
}
