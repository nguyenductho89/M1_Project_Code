#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

#include <fstream>
#include <string>

using namespace std;

class image{
private:
	int id_image, id_cluster;
	vector<double> values;
	int no_attr;
	string name;

public:
	image(int id_image, vector<double>& values, string name = ""){
		this->id_image = id_image;
		no_attr = values.size();
		for(int i = 0; i < no_attr; i++){
			this->values.push_back(values[i]);
		}
		this->name = name;
		id_cluster = -1;
	}
	
	int getID()	{ return id_image; }
	void setCluster(int id_cluster)	{ this->id_cluster = id_cluster;}
	int getCluster()	{ return id_cluster;}
	double getValue(int index)	{ return values[index]; }
	int getNoAttr()	{ return no_attr; }
	void addValue(double value)	{ values.push_back(value); }
	string getName()	{ return name; }
};

class Cluster{
private:
	int id_cluster;
	vector<double> central_values;
	vector<image> images;

public:
	Cluster(int id_cluster, image image){
		this->id_cluster = id_cluster;
		int no_attr = image.getNoAttr();
		for(int i = 0; i < no_attr; i++){
			central_values.push_back(image.getValue(i));
		}
		images.push_back(image);
	}

	void addimage(image image){
		images.push_back(image);
	}

	bool removeimage(int id_image){
		int total_images = images.size();
		for(int i = 0; i < total_images; i++){
			if(images[i].getID() == id_image){
				images.erase(images.begin() + i);
				return true;
			}
		}
		return false;
	}
	double getCentralValue(int index)	{ return central_values[index];}
	void setCentralValue(int index, double value) { central_values[index] = value; }
	image getimage(int index)	{	return images[index];}
	int getTotalimages()	{ return images.size(); }
	int getID()	{ return id_cluster; }
};

class KMeans{
private:
	int K; // number of clusters
	int no_attr, total_images;
	int max_iterations;
	vector<Cluster> clusters;

	// return ID of nearest center (uses euclidean distance)
	int getIDNearestCenter(image image){
		double sum = 0.0, min_dist;
		int id_cluster_center = 0;
		for(int i = 0; i < no_attr; i++){
			sum += pow(clusters[0].getCentralValue(i) -
					   image.getValue(i), 2.0);
		}
		min_dist = sqrt(sum);
		for(int i = 1; i < K; i++){
			double dist;
			sum = 0.0;
			for(int j = 0; j < no_attr; j++){
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
	//KMeans(int K, int total_images, int no_attr){
	KMeans(int K, int total_images, int no_attr, int max_iterations){
		this->K = K;
		this->total_images = total_images;
		this->no_attr = no_attr;
		//this->max_iterations = max_iterations;
	}
	void classify(vector<image> & images){
		if(K > total_images)
			return;
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
						clusters[id_old_cluster].removeimage(images[i].getID());

					images[i].setCluster(id_nearest_center);
					clusters[id_nearest_center].addimage(images[i]);
					done = false;
				}
			}
			// recalculating the center of each cluster
			for(int i = 0; i < K; i++){
				for(int j = 0; j < no_attr; j++){
					int total_images_cluster = clusters[i].getTotalimages();
					double sum = 0.0;
					if(total_images_cluster > 0){
						for(int p = 0; p < total_images_cluster; p++)
							sum += clusters[i].getimage(p).getValue(j);
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
			int total_images_cluster =  clusters[i].getTotalimages();
			cout << "Cluster " << clusters[i].getID() + 1 << endl;
			for(int j = 0; j < total_images_cluster; j++){
				cout << "image " << clusters[i].getimage(j).getID() + 1 << ": ";
				for(int p = 0; p < no_attr; p++){
					cout << clusters[i].getimage(j).getValue(p) << " ";
				}
				string image_name = clusters[i].getimage(j).getName();
				if(image_name != ""){ cout << "- " << image_name;}
				cout << endl;
			}
			cout << "Cluster values: ";
			for(int j = 0; j < no_attr; j++){
				cout << clusters[i].getCentralValue(j) << " ";
			}
			cout << "\n\n";
		}
	}
};

int main(int argc, char *argv[])
{
	srand (time(NULL));
	int total_images, no_attr, K, max_iterations, has_name;
	
	
	ifstream infile("dataset1.txt");
	string line;
	int counter;
	
	if (!infile.is_open()) //checks to see if file opens properly
	{
	    cerr << "Error: Could not find the requested file.";
	}else{
	    //if (getline(infile, line)) cout << line; // Get and print the line.
	    infile >> total_images >> no_attr >> K >> max_iterations >> has_name;
	   // cout << "total_images << no_attr << K << max_iterations << has_name: " << total_images << ", "<< no_attr<< ", " << K<< ", "<< max_iterations << ", " << has_name << "\n";
	    infile.close(); // Remember to close the file.
	}
	
	/*
	ofstream outfile("result.txt.txt");
	//outfile.open("result.txt");
	if(outfile.isopen()){
		outfile << "this is the content which will be writed\n";
	}
	*/
	
	//cin >> total_images >> no_attr >> K >> max_iterations >> has_name;
	vector<image> images;
	string image_name;

	for(int i = 0; i < total_images; i++){
		vector<double> values;
		if (!infile.is_open()){
				infile.ignore( 9, '\n' );
				double value;
			    while(infile >> value)
			    	values.push_back(value);    
			infile.close();
		}
		
		/*for(int j = 0; j < no_attr; j++){
			double value;
			cin >> value;
			values.push_back(value);
		}
		*/
		
		if(has_name){
			cin >> image_name;
			image p(i, values, image_name);
			images.push_back(p);
		}
		else{
			image p(i, values);
			images.push_back(p);
		}
	}
	KMeans kmeans(K, total_images, no_attr, max_iterations);
	kmeans.classify(images);
	
	return 0;
}
