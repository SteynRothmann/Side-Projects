#include <iostream>
#include <vector>
#include <conio.h>  // For _getch()

struct Point {
    int x, y;
    Point(int _x, int _y) : x(_x), y(_y) {}
};

// Function to rotate a point 90 degrees to the right
Point rotate90(Point p) {
    return Point(-p.y, p.x);
}

void drawCurve(const std::vector<Point>& curve) {
    // Find the range of x and y values for display
    int minX = curve[0].x, maxX = curve[0].x, minY = curve[0].y, maxY = curve[0].y;
    for (auto p : curve) {
        if (p.x < minX) minX = p.x;
        if (p.x > maxX) maxX = p.x;
        if (p.y < minY) minY = p.y;
        if (p.y > maxY) maxY = p.y;
    }

    // Print the curve to the console
    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            bool isLine = false;
            for (auto p : curve) {
                if (p.x == x && p.y == y) {
                    isLine = true;
                    break;
                }
            }
            if (isLine) std::cout << "#";  // Print line segment
            else std::cout << " ";        // Print empty space
        }
        std::cout << std::endl;
    }
}

int main() {
    std::vector<Point> curve;

    // Start with a single vertical line segment (0,0) -> (0,1)
    curve.push_back(Point(0, 0));
    curve.push_back(Point(0, 1));

    int iterations = 0;

    while (true) {
        system("CLS");  // Clear the console

        // Display the current iteration of the Dragon Curve
        std::cout << "Iteration: " << iterations << "\n";
        drawCurve(curve);

        // Wait for Enter key to be pressed
        std::cout << "\nPress Enter to continue, or 'q' to quit...";
        char ch = _getch();
        if (ch == 'q' || ch == 'Q') break;

        // Generate the next iteration of the Dragon Curve
        int size = curve.size();
        Point endPoint = curve.back();

        for (int j = size - 2; j >= 0; j--) {
            Point rotated = rotate90(Point(curve[j].x - endPoint.x, curve[j].y - endPoint.y));
            curve.push_back(Point(rotated.x + endPoint.x, rotated.y + endPoint.y));
        }

        iterations++;
    }

    return 0;
}
