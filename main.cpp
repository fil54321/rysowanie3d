#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath> 
#include <vector> 
#include <array> 

const int N = 500;
bool obraz[N][N] = {0};

const double pi = 3.141592;
const double straight = 180.0;

struct Point_2D { 
    double x;
    double y;
};

struct Point_3D { 
    double x;
    double y;
    double z;
};

struct Rotate { 
    double x;
    double y;
    double z;
};

struct Connections { 
    int first;
    int second;
};

void rotate3D(Point_3D point, Rotate angle, Point_2D& point2D) { 
    angle.x *= pi / straight; 
    angle.y *= pi / straight; 
    angle.z *= pi / straight; 

    double tx = point.x;
    double ty = point.y * cos(angle.x) - point.z * sin(angle.x);
    double tz = point.y * sin(angle.x) + point.z * cos(angle.x);

    point.x = tx * cos(angle.y) + tz * sin(angle.y);
    point.y = ty;
    point.z = tz * cos(angle.y) - tx * sin(angle.y);

    tx = point.x * cos(angle.z) - point.y * sin(angle.z);
    ty = point.x * sin(angle.z) - point.y * cos(angle.z);

    point2D.x = std::abs(tx);
    point2D.y = std::abs(ty);
}

void setPixel(int x, int y) {
    if (x >= 0 && x < N && y >= 0 && y < N) {
        obraz[x][y] = 1;
    }
}

void drawBresenham(int x1, int y1, int x2, int y2) {
    double e;
    int k_x = x1 < x2 ? 1 : -1;
    int k_y = y1 < y2 ? 1 : -1;
    int d_x = std::abs(x2 - x1);
    int d_y = std::abs(y2 - y1);
    setPixel(x1, y1);
    if (d_x >= d_y) {
        e = d_x / 2;
        for (int i = 0; i < d_x; i++) {
            x1 += k_x;
            e -= d_y;
            if (e < 0) {
                y1 += k_y;
                e += d_x;
            }
            setPixel(x1, y1);
        }
    } else {
        e = d_y / 2;
        for (int i = 0; i < d_y; i++) {
            y1 += k_y;
            e -= d_x;
            if (e < 0) {
                x1 += k_x;
                e += d_y;
            }
            setPixel(x1, y1);
        }
    }
}

int main() {
    std::ifstream input_file("wspolrzedne.txt");
    std::ofstream output_file("obraz.pbm");

    int number_of_tops, number_of_connections;
    input_file >> number_of_tops >> number_of_connections;

    double x, y, z;
    std::vector<Point_3D> points;
    for (int i = 0; i < number_of_tops; i++) { 
        input_file >> x >> y >> z;
        points.push_back({x, y, z});
    }
    
    int first, second;
    std::vector<Connections> points_to_connect;
    for (int i = 0; i < number_of_connections; i++) { 
        input_file >> first >> second;
        points_to_connect.push_back({first, second});
    }

    Rotate o = {10, 10, 0};
    Point_2D w;
    std::vector<Point_2D> points_2D; 
    for (double i = 0; i < number_of_tops; i++) { 
        rotate3D(points[(int)i], o, w);
        points_2D.push_back(w);
    }  

    for (int i = 0; i < number_of_connections; i++) { 
        drawBresenham(points_2D[points_to_connect[i].first].x, 
                      points_2D[points_to_connect[i].first].y, 
                      points_2D[points_to_connect[i].second].x, 
                      points_2D[points_to_connect[i].second].y);
    }
    
    output_file << "P1\n" << N << ' ' << N << '\n';
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            output_file << obraz[j][i];
        }
        output_file << '\n';
    }
    output_file.close();

    system("i_view64.exe obraz.pbm /convert=obraz.png");
    remove("obraz.pbm");
    system("i_view64.exe obraz.png");

    return 0;
}
