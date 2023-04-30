
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mapwidget.h"
#include <QToolButton>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QComboBox>
#include <QTextEdit>
#include <QVector>
#include <QMouseEvent>
#include <QDialog>
#include <QPixmap>
#include <QGridLayout>
#include <QLineEdit>
#include <QFileDialog>
#include <QHBoxLayout>


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void toolbar();   // definition for making a toolbar
    void buttons();   // here we declare the search and clear button actions
    void startXY(int X, int Y);    // both of these functions are for us so we can debug coordinates when we're placing them to make a graph
    void endXY(int X, int Y);
    void moveTOposition (int index);  //this functions coordinate will be used to guide the pen to mark on the map
    void backgroundpic();             // to show the map
public slots:
    void startcoordinates();          // slots that receive index of combobox start point corresponding to their coordinates on map
    void endcoordinates();            // slots that receive index of combobox finish point corresponding to their coordinates on map
    void paintpath();                 // receives path to paint it
    void clearpath();                 // clears current path by receiving a click on interface

private:
// various Qlibrary classes used to build interface and link with the functions
    QLabel *fromlabel;
    QLabel *Tolabel;
    QComboBox *startpoint;
    QComboBox *endpoint;
    QAction *buttonpresssearch;
    QAction *buttonpressclear;
    QGraphicsScene *scene;
    QGraphicsView *viewLecreusot;
    int FromX, FromY, toX, toY;
    QVector<int> pathforpaint;

    //this structure will carry the weight/edge of the graph
    struct distance
    {
        int weight;

    };


    static const int Numberofvertices = 200;   //an initialiser for various arrays, it does not correspond to actual vertices
    static const int maincomparator = 2500;  // has to be bigger than any edge

    //definition of struct is the adjacenecy matrix containing the distance object with a 2D array which will carry the corellation
    //which will correspond to its weight/distance
    struct AdjacencyMatrix
    {


        distance graphmatrix[Numberofvertices][Numberofvertices];
        int numberofpoints;  // actual number of points in graph

    };

    //main path finding class for the djkstra algorithm
    class Pathfinder
    {
    public:
        Pathfinder();  // initialiser and declaration of points
        AdjacencyMatrix matrix;   // adjacency matrix object declared
        void CreateGraph();       // graph weight and coordinates are here
        int previouspoint[Numberofvertices];    // array that receives inces in corelation to collecting the points
        int d[Numberofvertices];                 // array used for comparision and data input for djkstra
        bool nodeindex[Numberofvertices];        // receives the "true" val where the from point is so later the array can be used to detect collexted points
        void dijkstra(int fromcomboindex);       // main path finding algo
        QVector<int> storepath(int tocomboindex);   // stores path. is a vector because size isnt constant
    };
    Pathfinder *dj;              // pointer which is used so functions can be linked in the main window

protected:
    void mouseDoubleClickEvent (QMouseEvent *e);
};

#endif // MAINWINDOW_H






