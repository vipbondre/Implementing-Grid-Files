//Vipul Bondre 2020CSM1020
//Harshit Verma 2020CSM1011

#include <bits/stdc++.h> 
using namespace std;

// declaration of data structures and variables.
int no_of_records = 300; 
int x_last = 400, y_last = 400;
int no_of_buckets = 1;
int capacity = 10;
vector<int> X_partitions;
vector<int> Y_partitions;

class Node { 
public: 
    string f_name; 
    int size;
    int shared;
};

vector<pair<string, int>> file_names;
vector<Node*> file_names1;

vector<vector<int>> grid_matrix;
vector<int> dummy_vector;
////////////////////////////////////////////////////////

void create_dataset_1(int x_min ,int x_max ,int y_min , int y_max)
{
	ofstream fout , fdum;
    fout.open("sample_data.txt");  
    fdum.open("dummy.txt");
    srand(time(0));
    for(int i=1; i<=no_of_records; i++)
	{
		int x = rand();    
		x = x % x_last;      // x co-ordinate between 0 and 400
		int y = rand();
		y = y % y_last;      // y co-ordinate between 0 and 400 

		fout<<i<<" "<<x<<" "<<y<<endl;
		if(x >= x_min && x <= x_max && y >= y_min && y <= y_max)
		{
			fdum<<x<<" "<<y<<endl;
		}	
	}
	fout.close();
	fdum.close();
}

void create_dataset()
{
	ofstream fout;
    fout.open("sample_data.txt");
    vector<int> v, v1;
    for(int i=0; i<10; i++)
    {
    	v.push_back(i+1);
    	v1.push_back(i+1);
    }
    random_shuffle(v.begin(), v.end());
    random_shuffle(v1.begin(), v1.end());
    for(int i=0; i<10; i++)
	{
		int x = v[i];
		for(int j=0; j<10; j++)
		{
			int y = v1[j];
			fout<<i<<" "<<x<<" "<<y<<endl;
		}
	}
	fout.close();
}


int decode_line(string line, int* x, int* y , int z)
{
	int k=0;
	string xx = "", yy = "";
	if(line.size()<=1)
		return 0;

	if(z==0)
	{
		while(isdigit(line[k])) k++;
 		k++;
 	}
 	while(isdigit(line[k]))
 	{
 		xx.push_back(line[k]);
 		k++;
 	}
 	k++;
 	while(isdigit(line[k]))
 	{
 		yy.push_back(line[k]);
 		k++;
 	}

 	*x = stoi(xx);
 	*y = stoi(yy);

 	return 1;
}

int partition(string filename)
{

	ifstream f;
	f.open(filename);
	int x_min , x_max , y_min , y_max,k=0 , mid;

	for(int i=1; i<=capacity; i++)
	{
		string line;
		getline(f, line);

		int x, y;
		if(decode_line(line, &x, &y,1) == 0) break;
		if(k==0)
		{
			x_min =x;
			x_max =x ; 
			y_min = y;
			y_max =y;
			k++;
		}
		else
		{
			if(x>x_max)
				x_max=x;
			else if (x<x_min)
				x_min= x;

			if(y>y_max)
				y_max=y;
			else if (y<y_min)
				y_min= y;
		}
	}
	f.close();

	if ((x_max-x_min) > (y_max-y_min))
	{	
		mid = (x_max+x_min)/2;
		//X_partitions.push_back(mid);
	    //sort(X_partitions.begin(), X_partitions.end());
		int i=0;
		for(int j=0; j<X_partitions.size(); j++)
		{
			if(mid < X_partitions[j])
			{
				i = j;
				break;
			}
		}
		X_partitions.insert(X_partitions.begin()+i, mid);

		//cout<<"x_max "<<x_max<<"x_min "<<x_min<<"  Mid (x) = "<<mid<<"  index "<<i<<endl;
		return i;  
	}
	else
	{
		mid = (y_max+y_min)/2;
		//Y_partitions.push_back(mid);
 		//sort(Y_partitions.begin(), Y_partitions.end());
 		int i=0;
		for(int j=0; j<Y_partitions.size(); j++)
		{
			if(mid < Y_partitions[j])
			{
				i = j;
				break;
			}
		}
		Y_partitions.insert(Y_partitions.begin()+i, mid);

		//cout<<"y_max "<<y_max<<"y_min "<<y_min<<"  Mid (y) = "<<mid<<"  index "<<i<<endl;
		return i;
	}
}

void get_cell(int x, int y, int* i, int* j)
{
	for(int a = 0; a < X_partitions.size(); a++)
	{
		if(x < X_partitions[a]) 
		{
			*j = a-1;
			break;
		}
	}
	for(int b = 0; b < Y_partitions.size(); b++)
	{
		if(y < Y_partitions[b]) 
		{
			*i = b-1;
			break;
		}
	}
}

void write_into_bucket(string filename, int x, int y)
{
	fstream f;
	f.open(filename, std::ios::app);
	f<<x<<" "<<y<<endl;
	f.close();
}

int reconstruct_grid(int split_pos)
{
	int m = X_partitions.size(), n = Y_partitions.size();
	m--; n--;

	if(grid_matrix.size() == n)
	{
		for(int i=0; i<grid_matrix.size(); i++)
		{
			grid_matrix[i].insert(grid_matrix[i].begin()+split_pos, grid_matrix[i][split_pos-1]);
			file_names1[grid_matrix[i][split_pos-1]]->shared += 1;
		}
		return 2;
	}
	else
	{
		for(int i=0; i<grid_matrix[0].size(); i++)
		{
			dummy_vector.push_back(grid_matrix[split_pos-1][i]);
			file_names1[grid_matrix[split_pos-1][i]]->shared += 1;
		}
		grid_matrix.insert(grid_matrix.begin()+split_pos, dummy_vector);
		dummy_vector.clear();
	}
	return 1;
}


void create_bucket(int grid_split, int x, int y, int i, int j)
{
	int a = file_names.size(), b, x_min, x_max, y_max, y_min, size=0, m, n;
	string line, new_bucket, temp;
	
	ifstream f1;
	ofstream f, f2;
	new_bucket= std::to_string(a) + ".txt";
	f2.open(new_bucket);
	file_names.push_back(make_pair(new_bucket, 0));
	Node *nn = new Node();   nn->f_name = new_bucket;   nn->size = 0;  nn->shared = 1;
	file_names1.push_back(nn);
	
	f.open("temp.txt");	
	b = grid_matrix[i][j];
	f1.open(file_names[b].first);

	temp = file_names[b].first ;
	char * old_bucket = new char [temp.length()+1];
	strcpy (old_bucket, temp.c_str());		

	file_names1[grid_matrix[i][j]]->shared -= 1;
	if(grid_split==1)
	{ 	
		i++;
		x_min = X_partitions[j];           x_max = X_partitions[j+1];
		y_min = Y_partitions[i];         y_max = Y_partitions[i+1];
			
	}
	else if(grid_split == 2)
	{	
		j++;
		x_min = X_partitions[j];        x_max = X_partitions[j+1];
		y_min = Y_partitions[i]; 		  y_max = Y_partitions[i+1];	
	}
	else
	{
		x_min = X_partitions[j];        x_max = X_partitions[j+1];
		y_min = Y_partitions[i]; 		  y_max = Y_partitions[i+1];
	}
	grid_matrix[i][j] = a;

	//cout<<"x_min, x_max, y_min, y_max "<<x_min<<","<<x_max<<","<<y_min<<","<<y_max<<endl;

	for(int c=1; c<=no_of_records; c++)
	{
		getline(f1, line);
		if(decode_line(line, &m, &n, 1) == 0) break;

		if(m >= x_min && m <= x_max && n >= y_min && n <= y_max)
		{
			write_into_bucket(new_bucket, m, n);
			file_names[grid_matrix[i][j]].second++;
			file_names1[grid_matrix[i][j]]->size += 1;
			if(file_names[grid_matrix[i][j]].second++> capacity)
			{
				//cout<<"*******************Warning***********************"<<endl;
			}
		}
		else
		{
			write_into_bucket("temp.txt", m, n);
			size++;
		}
	}

	f1.close();
	f.close();
	f2.close();
	
	//remove the original file
    remove(old_bucket);
  
    // rename the file
    if(rename("temp.txt", old_bucket) != 0)
		perror("Error renaming file");

	file_names[b].second = size;
	file_names1[b]->size = size;
}


void range_query(int x_min , int x_max ,int y_min , int y_max)
{

	int m = -1, m_max =-1 , n=-1 , n_max=-1 , b , b_size , x=-1 ,y=-1 ,i;
	string line;
	ifstream f ;
	ofstream f_out;
	f_out.open("output.txt");
	vector<int> vec;

	for( i=0; i< X_partitions.size();i++)
	{
		if(X_partitions[i] <= x_min)
		{
			m++;
		}
		if(X_partitions[i] == x_min && m >= 1) m--;
		if (X_partitions[i] < x_max)
		{
			m_max++;
		}
		if(X_partitions[i] == x_max)
		{
			if(i != X_partitions.size()-1) m_max++;
		}
	}

	for( i=0; i< Y_partitions.size();i++)
	{
		if(Y_partitions[i] <= y_min )
		{
			n++ ;
		}
		if(Y_partitions[i] == y_min && n >= 1) n--;
		if (Y_partitions[i] < y_max)
		{
			n_max++;
		}
		if(Y_partitions[i] == y_max)
		{
			if(i != Y_partitions.size()-1) n_max++;
		}
	}

	int h=1;
	//cout << m <<" "<<m_max<<" "<<n<<" "<<n_max;

	for(int u = n ;u<=n_max;u++)
	{
		//cout << "check "<<endl; 
		for(int v= m; v<=m_max;v++)
		{
			//cout<<" check 2"<<endl;
			b= grid_matrix[u][v];

			//cout<< " bucket = "<<b<<endl;
		    for(int p=0;p<vec.size();p++)
		    {
		    	if(vec[p]==b)
		    		h=0;		    		
			}

			if(h)
		   	{
	    		vec.push_back(b);
				f.open(file_names[b].first);
				//cout<<file_names[b].first<<endl;
				for(int a =1; a <= file_names[b].second; a++)
				{
					getline(f, line);
					//cout<<"line = "<<line<<endl;
					if(decode_line(line, &x, &y, 1) == 0) break;
					//cout<< x << "  "<<y <<endl;

					if(x >= x_min && x <= x_max && y >= y_min && y <= y_max)
					{
						write_into_bucket("output.txt", x, y);
						//cout<< x << "  "<<y <<endl;					
					}
				}
				f.close();
			}
			h=1;
		}
	}
	f_out.close();
}

void print_grid()
{
	for(int i=0; i<grid_matrix.size(); i++)
	{
		for(int j=0; j<grid_matrix[0].size(); j++)
		{
			cout<<grid_matrix[i][j]<<" ";
		}
		cout<<endl;
	}
}

int main()
{
	//intialize X and Y partitions
	X_partitions.push_back(0);  X_partitions.push_back(x_last);
	Y_partitions.push_back(0);  Y_partitions.push_back(y_last);

	int m = X_partitions.size(), n = Y_partitions.size();
	m--; n--;
	//////////////////////////////////////////////////////////////////
	cout<<"Enter total no of records: ";
	cin>>no_of_records;
	cout<<endl<<"Enter capacity of the bucket: ";
	cin>>capacity;


	//////////////////////////////////////////////////////////////////
	int r_x_min , r_x_max ,r_y_min , r_y_max ;

	cout<<endl<<"Enter co-ordinates of range query "<< endl <<"x_min = ";
	cin >> r_x_min;
	cout<<endl<<"x_max = " ;
	cin >> r_x_max;
	cout<<endl<<"y_min = " ;
	cin >> r_y_min;
	cout<<endl<<"y_max = ";
	cin >> r_y_max;
	/////////////////////////////////////////////////////////////////////

	///////////////cout<<m<<" "<<n<<endl;

	string b;

	///initialize bucket and vector of bucket names.
	ofstream f;
    f.open("0.txt");
    f.close();
	file_names.push_back(make_pair("0.txt", 0));
	Node *nn = new Node();   nn->f_name = "0.txt";   nn->size = 0;  nn->shared = 1;
	file_names1.push_back(nn);
	//////////////////////////////////////////////////////

	////////////// initialize grid matrix;
    dummy_vector.push_back(0);
    grid_matrix.push_back(dummy_vector);
    dummy_vector.clear();
    /////////////////////////////////////////////

    ////creation of initial 


   create_dataset_1(r_x_min , r_x_max ,r_y_min , r_y_max);
    /////////////////////////////
    
    ifstream file;
	file.open("sample_data.txt");
	for(int c=1; c<=no_of_records; c++)
	{
		string line;
		getline(file, line);
		int x, y;
		if(decode_line(line, &x, &y,0) == 0) break;
		//////////////////////////////////////////cout<<x<<" "<<y<<endl;
		int i, j;
		get_cell(x, y, &i, &j);

		/////////////////////////////////////////cout<<i<<" "<<j<<endl;
		if(file_names[grid_matrix[i][j]].second < capacity)
		{
			write_into_bucket(file_names[grid_matrix[i][j]].first, x, y);
			file_names[grid_matrix[i][j]].second++;
			file_names1[grid_matrix[i][j]]->size += 1;
		}
		else if(file_names1[grid_matrix[i][j]]->shared == 1)
		{
			//cout<<"i and j are "<<i<<" "<<j<<endl;

			//////////// partiton function
			int split_pos = partition(file_names[grid_matrix[i][j]].first);		

			/////////// split & reconstruct
			int grid_split = reconstruct_grid(split_pos); // 1 for row , 2 for column 
			//cout<<"grid reconstructed with rows = "<<grid_matrix.size()<<" and cols = "<<grid_matrix[0].size()<<endl;
				
			write_into_bucket(file_names[grid_matrix[i][j]].first, x, y);
			file_names[grid_matrix[i][j]].second++;
			file_names1[grid_matrix[i][j]]->size += 1;
			
			/////////////// create new bucket & divide data
			create_bucket(grid_split, x, y, i, j);
		}
		else
		{
			write_into_bucket(file_names[grid_matrix[i][j]].first, x, y);
			file_names[grid_matrix[i][j]].second++;
			file_names1[grid_matrix[i][j]]->size += 1;
			create_bucket(0, x, y, i, j);

			//cout<<"only physical partition done....!!!!!!"<<endl;
		}
    }

    int count=0;
    for (int i = 0; i < file_names.size(); i++)
    {
    	//cout << file_names[i].first <<"  "<< file_names[i].second <<endl;
    	//cout<<file_names1[i]->f_name<<"  "<<file_names1[i]->size<<"  "<<file_names1[i]->shared<<endl;
    	count += file_names1[i]->size;
    }

    cout<<endl<<"The grid:"<<endl;
    print_grid();
	cout<<endl;

	range_query(r_x_min , r_x_max ,r_y_min , r_y_max);

	cout<<"Range query coordinates: ";
    cout<<r_x_min <<"  "<< r_x_max <<" "<<r_y_min <<"  "<< r_y_max<<endl;
    cout<<"X_partitions: ";
    for(int ab= 0; ab< X_partitions.size();ab++)
    {
    	cout<<X_partitions[ab]<< "  ";
    } 
	cout<<endl<<"Y_partitions: ";
	for(int ab= 0; ab< Y_partitions.size();ab++)
    {
    	cout<<Y_partitions[ab]<< "  ";
    }
    cout<<endl<<endl;

    //cout<<"total records are "<<count<<endl;

  	cout<<"See the output.txt file for the range query output."<<endl;

    return 0;
}