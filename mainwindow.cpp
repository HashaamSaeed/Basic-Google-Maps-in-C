/*
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//Syed Muhammad Hashaam Saeed
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

// please ignore the debugging !!! i included it since it helped me till the end to improve my code
*/
#include "mainwindow.h"
#include <qdebug.h>
#include <QToolBar>
#include <QtAlgorithms>
#include <iostream>
#include <windows.h>

using namespace std;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)  // this is where we use tge pathfinder pointer to create the graph on mainwindow so path can be found
{

    setWindowTitle ("Welcome to Le creusot");
    dj = new MainWindow::Pathfinder(); // loads the pathfinder class and function to work on mainwindow
    dj->CreateGraph ();               // creates graph on mainwindow

    scene = new QGraphicsScene;        // setting for graphicsscene
    scene->setSceneRect (-100, -100, 700, 700);

    backgroundpic();
    viewLecreusot = new QGraphicsView;
    viewLecreusot->setScene (scene);
    viewLecreusot->setMinimumSize (1920,1080);  // my laptops screen resolution
    viewLecreusot->show ();
    setCentralWidget (viewLecreusot);
    buttons ();
    toolbar ();
    setMinimumSize (1920, 1080);
    Sleep(2000);

}


//Fills the graph with maincomparator at every value from [0][0] to [106][106] for it to be used as a comparator in the logic in dijkstra
MainWindow::Pathfinder::Pathfinder()
{
    matrix.numberofpoints = 107;  // total points in our created graph

    for (int i = 0; i < matrix.numberofpoints; i++)
       {
           for (int j = 0; j < matrix.numberofpoints; j++)
           {

                   matrix.graphmatrix[i][j].weight = maincomparator;

           }
       }


}


//fromcomboindex is the index of the starting point drop down window index
// the main path finding algo
void MainWindow::Pathfinder::dijkstra (int fromcomboindex)
{

      // matrix.numberofpoints = 107
    for (int i = 0; i < matrix.numberofpoints; i++) d[i] = maincomparator; // populates d[i] with maincomparator from 0-106
    for (int i = 0; i < matrix.numberofpoints; i++) nodeindex[i] = false;// populates bool nodeindex[i] with 0 from 0-106
    for (int i = 0; i < matrix.numberofpoints; i++) previouspoint[i] = -1; // populates previouspoint[i] with -1 from 0-106
    d[fromcomboindex] = 0;// makes value at d[fromcomboindex] to be zero for it to fail in a comparision below which allocates weight
                          // in the matrix later on , this point acts as a stopping point to store the paths weights

    while (true)    // will always run for once no matter what, which is why sometimes whwen you search for a path the program is buggy
                   // and will jump to some unknown path at start but then gets alright after we press the search button again
    {
        int v = -1;     // declaring v for it to work for at least the first fromcombobox index

        for (int u = 0; u < matrix.numberofpoints; u++)  // value of d[fromcomboindex] makes it break the loop because it replaces maincomparator
        {
           // qDebug()<<"u  "<<u;
            if (!nodeindex[u] && (v == -1 || d[u] <= d[v]))  // since u is always +1 than v therefore at d[fromcomboindex] it will be less than maincomparator
               // cout<<"v  "<<v<<"  u  "<<u<<endl;          // since d[fromcomboindex]=0 and there the loop will break and move on
              // qDebug()<<"  d[u]  "<<d[u]<<"  d[v]  "<<d[v]<<"end1";
                v = u;
               //std::cout<<"v1  "<<v<<"  u1  "<<u<<endl;
           //qDebug()<<"  d[u]  "<<d[u]<<"  d[v]  "<<d[v]<<"end2";
        }

        if (v == -1) break;         // this is to check if program ran accidently, in QT combobox if there's nothing then default value is always -1 so this
                                    // is to make it exit the function and everything

        //cout<<"v  "<<v;
        nodeindex[v] = true;     // when the for loop above breaks the val of v that it broke on becomes
                                // index to make true at that value in the nodeindex array
        for (int u = 0; u < matrix.numberofpoints; u++)
        {    //qDebug()<<"v"<<v;
            if (d[u] > d[v] + matrix.graphmatrix[v][u].weight)// condition is asking if maincomparator > edge at graphmatrix[v][u] ?
                                                              // i don't know why it just doenst accept matrix.graphmatrix[][].weight yohan
            { // qDebug()<<"u"<<u;                            // you have to add d[] before it to compare the distance to maincomparator value
             // qDebug()<<"d[u]  "<<d[u]<<"  d[v]  "<<d[v]+matrix.graphmatrix[v][u].weight<<"hell1";
                d[u] = d[v] + matrix.graphmatrix[v][u].weight;  // assigns weight of desired path to the vector d[]
              //qDebug()<<"d[u]  "<<d[u]<<"  d[v]  "<<d[v]+matrix.graphmatrix[v][u].weight<<"hell2";
                previouspoint[u] = v;                                //replaces the -1 value with the weight of the corresponding path
               // qDebug()<<"v"<<v;
            }
        }
    }
}



//definition of storepath stores path from start to end
//tocomboindex here is the index of the drop down finishing point menu
QVector<int> MainWindow::Pathfinder::storepath (int tocomboindex)
{
    //cout<<"minus  "<<previouspoint[0]<<endl;
    //cout<<"  one  "<<previouspoint[tocomboindex]<<endl;
    QVector<int> path;  // vector to store the path in

//std::cout<< "tocomboindex"<<tocomboindex;
    for ( ; tocomboindex != -1; tocomboindex = previouspoint[tocomboindex])// -1 is stored everywhere in previouspoint[] and only the first elements have values of v
                                                                            // which is calculated in the above function therefore it prescribes the endpoint index
    {                                                                       // as the limit to the path
      //   cout<<"two  "<<previouspoint[tocomboindex]<<endl;
       //std::cout << "tocomboindex: " << tocomboindex << ", ";
        path.push_back (tocomboindex);                              // stores the opposite path
 //cout<<"three  "<<previouspoint[tocomboindex]<<endl;
    }
// reverses the data stream in path vector since the above for loop while pushing data into the path vector is actually the opposite
// path desired by the user therefore now it'll reverse it, although it doenst matter since the painted path will be the same but
// we have to make sure for projects sake that the path found is real with the right direction
    std::reverse(path.begin (), path.end ());
    return path; //returns the path vector

}



// this is the green flag button on the main screen
void MainWindow::paintpath ()
{
//so here is the search button but i named it paint path because thats what it does actually
// firstly it uses the pathfinder pointer to activate our dijkstra() function in order to begin searching the shortest path
    dj->dijkstra (startpoint->currentIndex ());
    pathforpaint = dj->storepath (endpoint->currentIndex ()); //reversed path from the above function becomes nextPath which is the path
                                                              // that the painter has to paint
    clearpath ();   // clears old path being displayed
    QGraphicsPathItem *item = new QGraphicsPathItem();
    QPen pen;
    pen.setWidth (5);
    pen.setColor (Qt::green);
    item->setPen (pen);
    item->setFlag (QGraphicsItem::ItemIsPanel);

// qDebug() << FromX << " " << FromY << " " << toX << " " << toY;
    //qDebug() << "Hello World !";
    /*
    for (int i = 0; i < pathforpaint.size (); i++)                        // for my debugging
    {
      //qDebug() << pathforpaint[i] << " , ";

    }
*/
    scene->addItem (item);

    QPainterPath pa;               // initialising painter path class with pa as an object
    pa.moveTo (FromX-650 , FromY-290 );  // starting point of pen stroke with the adjusted coordinates corresponding to starting pos
    for (int i = 1; i < pathforpaint.size() ; i++)
    {
        moveTOposition (pathforpaint[i]);             // this guides the pen to the next coordinates on the designates paths vetice thhat its near to
        pa.lineTo (toX-650, toY-290);  // ending point of pen stroke with the adjusted ending coordinates
    }
item->setPath (pa);

}



// initialises X,Y startpoints depending on set coordinates in functions below
void MainWindow::startXY(int X, int Y)
{
    FromX = X; FromY = Y;
    // qDebug() << X << ", " << Y;
}

// initialises X,Y endpoints depending on set coordinates in functions below
void MainWindow::endXY (int X, int Y)
{
    toX = X; toY = Y;
  //  qDebug() << X << ", " << Y;
}





//setting Le creusot picture on the mainwindow to be displayed when we open the app
void MainWindow::backgroundpic ()
{
    QGraphicsPixmapItem *item =
            scene->addPixmap (QPixmap(":/images/IMG/LeC.png"));
// allows pic to be moved by mouse =  item->setFlag (QGraphicsItem::ItemIsMovable);
    //item->setPos(0,0);
    item->setPos (-650, -290);
}


//mainwindow destructor
MainWindow::~MainWindow()
{

}


// the 2 buttons for search and clear declarations and their small logo on the mainwindow
void MainWindow::buttons ()
{
    buttonpresssearch = new QAction(QIcon(":/images/IMG/bino.png"),tr("Search"), this);
    connect (buttonpresssearch, SIGNAL(triggered(bool)), this, SLOT(paintpath())); //tells the paintpath() function to be activated once clicked

    buttonpressclear = new QAction(QIcon(":/images/IMG/erase.png"), tr("Clear"), this);
    connect (buttonpressclear, SIGNAL(triggered(bool)), this, SLOT(clearpath()));        //tells the clearpath() function to be activated once clicked


}



// this clears path everytime a new path is asked by the user therefore image needs to be rearranged as seen the declaration of the main
// background image
void MainWindow::clearpath ()
{
    QList<QGraphicsItem*> listItem = scene->items ();
    while (!listItem.empty ())
    {
        scene->removeItem (listItem.at (0));
        listItem.removeAt (0);
    }

    QGraphicsPixmapItem *item =
            scene->addPixmap (QPixmap(":/images/IMG/LeC.png"));
//    item->setFlag (QGraphicsItem::ItemIsMovable);  // lets the user move the backgroud image by mouse
    item->setPos (-650, -290);

}




// just a mouse click event that gives the coordinates of the main window, therefore when we attaches the photo we had to adjust the points
void MainWindow::mouseDoubleClickEvent (QMouseEvent *e)
{

    qDebug() << "x : " << e->x () << ", y : " << e->y ();
    cout<<"X:"<<e->x();

}


// main function where the graph is declared, here the value of the 2D array corresponds to the point on the graph and its corresponding
// weight/distance
void MainWindow::Pathfinder::CreateGraph ()
{
    matrix.graphmatrix[1][0].weight = matrix.graphmatrix[0][1].weight = 625;
    matrix.graphmatrix[1][2].weight = matrix.graphmatrix[2][1].weight = 159;
    matrix.graphmatrix[1][3].weight = matrix.graphmatrix[3][1].weight = 341;
    matrix.graphmatrix[2][1].weight = matrix.graphmatrix[1][2].weight = 159;
    matrix.graphmatrix[2][4].weight = matrix.graphmatrix[4][2].weight = 344;
    matrix.graphmatrix[3][1].weight = matrix.graphmatrix[1][3].weight = 341;
    matrix.graphmatrix[3][5].weight = matrix.graphmatrix[5][3].weight = 145;
    matrix.graphmatrix[4][2].weight = matrix.graphmatrix[2][4].weight = 344;
    matrix.graphmatrix[4][7].weight = matrix.graphmatrix[7][4].weight = 145;
    matrix.graphmatrix[4][8].weight = matrix.graphmatrix[8][4].weight = 76;
    matrix.graphmatrix[5][3].weight = matrix.graphmatrix[3][5].weight = 145;
    matrix.graphmatrix[5][6].weight = matrix.graphmatrix[6][5].weight = 170;
    matrix.graphmatrix[6][5].weight = matrix.graphmatrix[5][6].weight = 170;
    matrix.graphmatrix[6][7].weight = matrix.graphmatrix[7][6].weight = 123;
    matrix.graphmatrix[6][106].weight = matrix.graphmatrix[106][6].weight = 98;
    matrix.graphmatrix[7][4].weight = matrix.graphmatrix[4][7].weight = 145;
    matrix.graphmatrix[7][6].weight = matrix.graphmatrix[6][7].weight = 123;
    matrix.graphmatrix[7][8].weight = matrix.graphmatrix[8][7].weight = 128;
    matrix.graphmatrix[7][10].weight = matrix.graphmatrix[10][7].weight = 218;
    matrix.graphmatrix[8][4].weight = matrix.graphmatrix[4][8].weight = 76;
    matrix.graphmatrix[8][7].weight = matrix.graphmatrix[7][8].weight = 128;
    matrix.graphmatrix[8][9].weight = matrix.graphmatrix[9][8].weight = 309;
    matrix.graphmatrix[8][85].weight = matrix.graphmatrix[85][8].weight = 186;
    matrix.graphmatrix[9][8].weight = matrix.graphmatrix[8][9].weight = 309;
    matrix.graphmatrix[9][11].weight = matrix.graphmatrix[11][9].weight = 90;
    matrix.graphmatrix[10][7].weight = matrix.graphmatrix[7][10].weight = 218;
    matrix.graphmatrix[10][11].weight = matrix.graphmatrix[11][10].weight = 80;
   // matrix.graphmatrix[10][15].weight = matrix.graphmatrix[15][10].weight = 645;
    matrix.graphmatrix[11][9].weight = matrix.graphmatrix[9][11].weight = 90;
    matrix.graphmatrix[11][10].weight = matrix.graphmatrix[10][11].weight = 80;
    matrix.graphmatrix[11][12].weight = matrix.graphmatrix[12][11].weight = 93;
    matrix.graphmatrix[12][11].weight = matrix.graphmatrix[11][12].weight = 93;
    matrix.graphmatrix[12][13].weight = matrix.graphmatrix[13][12].weight = 261;
    matrix.graphmatrix[13][12].weight = matrix.graphmatrix[12][13].weight = 261;
    matrix.graphmatrix[13][14].weight = matrix.graphmatrix[14][13].weight = 202;
    matrix.graphmatrix[14][13].weight = matrix.graphmatrix[13][14].weight = 202;
    matrix.graphmatrix[14][15].weight = matrix.graphmatrix[15][14].weight = 124;
    //matrix.graphmatrix[15][10].weight = matrix.graphmatrix[10][15].weight = 645;
    matrix.graphmatrix[15][14].weight = matrix.graphmatrix[14][15].weight = 124;
    matrix.graphmatrix[15][26].weight = matrix.graphmatrix[26][15].weight = 124;
    matrix.graphmatrix[16][22].weight = matrix.graphmatrix[22][16].weight = 186;
    matrix.graphmatrix[16][86].weight = matrix.graphmatrix[86][16].weight = 68;
    matrix.graphmatrix[17][18].weight = matrix.graphmatrix[18][17].weight = 182;
    matrix.graphmatrix[17][87].weight = matrix.graphmatrix[87][17].weight = 79;
    matrix.graphmatrix[17][88].weight = matrix.graphmatrix[88][17].weight = 613;
    matrix.graphmatrix[18][17].weight = matrix.graphmatrix[17][18].weight = 182;
    matrix.graphmatrix[18][19].weight = matrix.graphmatrix[19][18].weight = 176;
    matrix.graphmatrix[18][20].weight = matrix.graphmatrix[20][18].weight = 76;
    matrix.graphmatrix[19][18].weight = matrix.graphmatrix[18][19].weight = 176;
    matrix.graphmatrix[19][25].weight = matrix.graphmatrix[25][19].weight = 77;
    matrix.graphmatrix[19][29].weight = matrix.graphmatrix[29][19].weight = 259;
    matrix.graphmatrix[20][18].weight = matrix.graphmatrix[18][20].weight = 76;
    matrix.graphmatrix[20][21].weight = matrix.graphmatrix[21][20].weight = 65;
    matrix.graphmatrix[20][25].weight = matrix.graphmatrix[25][20].weight = 165;
    matrix.graphmatrix[20][87].weight = matrix.graphmatrix[87][20].weight = 187;
    matrix.graphmatrix[21][20].weight = matrix.graphmatrix[20][21].weight = 65;
    matrix.graphmatrix[21][22].weight = matrix.graphmatrix[22][21].weight = 55;
    matrix.graphmatrix[21][24].weight = matrix.graphmatrix[24][21].weight = 167;
    matrix.graphmatrix[21][86].weight = matrix.graphmatrix[86][21].weight = 187;
    matrix.graphmatrix[22][16].weight = matrix.graphmatrix[16][22].weight = 186;
    matrix.graphmatrix[22][21].weight = matrix.graphmatrix[21][22].weight = 55;
    matrix.graphmatrix[22][23].weight = matrix.graphmatrix[23][22].weight = 162;
    matrix.graphmatrix[23][22].weight = matrix.graphmatrix[22][23].weight = 162;
    matrix.graphmatrix[23][24].weight = matrix.graphmatrix[24][23].weight = 61;
    matrix.graphmatrix[23][27].weight = matrix.graphmatrix[27][23].weight = 144;
    matrix.graphmatrix[23][28].weight = matrix.graphmatrix[28][23].weight = 266;
    matrix.graphmatrix[24][21].weight = matrix.graphmatrix[21][24].weight = 167;
    matrix.graphmatrix[24][23].weight = matrix.graphmatrix[23][24].weight = 61;
    matrix.graphmatrix[24][25].weight = matrix.graphmatrix[25][24].weight = 66;
    matrix.graphmatrix[25][19].weight = matrix.graphmatrix[19][25].weight = 77;
    matrix.graphmatrix[25][20].weight = matrix.graphmatrix[20][25].weight = 165;
    matrix.graphmatrix[25][24].weight = matrix.graphmatrix[24][25].weight = 66;
    matrix.graphmatrix[26][15].weight = matrix.graphmatrix[15][26].weight = 124;
    matrix.graphmatrix[26][27].weight = matrix.graphmatrix[27][26].weight = 298;
    matrix.graphmatrix[27][23].weight = matrix.graphmatrix[23][27].weight = 144;
    matrix.graphmatrix[27][26].weight = matrix.graphmatrix[26][27].weight = 298;
    matrix.graphmatrix[27][32].weight = matrix.graphmatrix[32][27].weight = 207;
    matrix.graphmatrix[28][23].weight = matrix.graphmatrix[23][28].weight = 266;
    matrix.graphmatrix[28][29].weight = matrix.graphmatrix[29][28].weight = 199;
    matrix.graphmatrix[28][30].weight = matrix.graphmatrix[30][28].weight = 123;
    matrix.graphmatrix[28][32].weight = matrix.graphmatrix[32][28].weight = 245;
    matrix.graphmatrix[29][19].weight = matrix.graphmatrix[19][29].weight = 259;
    matrix.graphmatrix[29][28].weight = matrix.graphmatrix[28][29].weight = 199;
    matrix.graphmatrix[29][31].weight = matrix.graphmatrix[31][29].weight = 155;
    matrix.graphmatrix[30][28].weight = matrix.graphmatrix[28][30].weight = 123;
    matrix.graphmatrix[30][31].weight = matrix.graphmatrix[31][30].weight = 148;
    matrix.graphmatrix[30][33].weight = matrix.graphmatrix[33][30].weight = 223;
    matrix.graphmatrix[30][35].weight = matrix.graphmatrix[35][30].weight = 507;
    matrix.graphmatrix[31][29].weight = matrix.graphmatrix[29][31].weight = 155;
    matrix.graphmatrix[31][30].weight = matrix.graphmatrix[30][31].weight = 148;
    matrix.graphmatrix[31][34].weight = matrix.graphmatrix[34][31].weight = 583;
    matrix.graphmatrix[32][27].weight = matrix.graphmatrix[27][32].weight = 207;
    matrix.graphmatrix[32][28].weight = matrix.graphmatrix[28][32].weight = 245;
    matrix.graphmatrix[32][33].weight = matrix.graphmatrix[33][32].weight = 177;
    matrix.graphmatrix[32][47].weight = matrix.graphmatrix[47][32].weight = 320;
    matrix.graphmatrix[33][30].weight = matrix.graphmatrix[30][33].weight = 223;
    matrix.graphmatrix[33][32].weight = matrix.graphmatrix[32][33].weight = 177;
    matrix.graphmatrix[33][38].weight = matrix.graphmatrix[38][33].weight = 360;
    matrix.graphmatrix[33][46].weight = matrix.graphmatrix[46][33].weight = 147;
    matrix.graphmatrix[34][31].weight = matrix.graphmatrix[31][34].weight = 583;
    matrix.graphmatrix[34][35].weight = matrix.graphmatrix[35][34].weight = 44;
    matrix.graphmatrix[34][36].weight = matrix.graphmatrix[36][34].weight = 136;
    matrix.graphmatrix[35][30].weight = matrix.graphmatrix[30][35].weight = 507;
    matrix.graphmatrix[35][34].weight = matrix.graphmatrix[34][35].weight = 44;
    matrix.graphmatrix[35][36].weight = matrix.graphmatrix[36][35].weight = 128;
    matrix.graphmatrix[35][38].weight = matrix.graphmatrix[38][35].weight = 55;
    matrix.graphmatrix[36][34].weight = matrix.graphmatrix[34][36].weight = 136;
    matrix.graphmatrix[36][35].weight = matrix.graphmatrix[35][36].weight = 128;
    matrix.graphmatrix[36][37].weight = matrix.graphmatrix[37][36].weight = 64;
    matrix.graphmatrix[36][39].weight = matrix.graphmatrix[39][36].weight = 106;
    matrix.graphmatrix[37][36].weight = matrix.graphmatrix[36][37].weight = 64;
    matrix.graphmatrix[37][38].weight = matrix.graphmatrix[38][37].weight = 111;
    matrix.graphmatrix[37][39].weight = matrix.graphmatrix[39][37].weight = 89;
    matrix.graphmatrix[37][43].weight = matrix.graphmatrix[43][37].weight = 181;
    matrix.graphmatrix[38][33].weight = matrix.graphmatrix[33][38].weight = 360;
    matrix.graphmatrix[38][35].weight = matrix.graphmatrix[35][38].weight = 55;
    matrix.graphmatrix[38][37].weight = matrix.graphmatrix[37][38].weight = 111;
    matrix.graphmatrix[38][44].weight = matrix.graphmatrix[44][38].weight = 139;
    matrix.graphmatrix[39][36].weight = matrix.graphmatrix[36][39].weight = 106;
    matrix.graphmatrix[39][37].weight = matrix.graphmatrix[37][39].weight = 89;
    matrix.graphmatrix[39][40].weight = matrix.graphmatrix[40][39].weight = 115;
    matrix.graphmatrix[39][42].weight = matrix.graphmatrix[42][39].weight = 173;
    matrix.graphmatrix[40][39].weight = matrix.graphmatrix[39][40].weight = 115;
    matrix.graphmatrix[40][41].weight = matrix.graphmatrix[41][40].weight = 91;
    matrix.graphmatrix[40][64].weight = matrix.graphmatrix[64][40].weight = 417;
    matrix.graphmatrix[41][40].weight = matrix.graphmatrix[40][41].weight = 91;
    matrix.graphmatrix[41][42].weight = matrix.graphmatrix[42][41].weight = 84;
    matrix.graphmatrix[41][51].weight = matrix.graphmatrix[51][41].weight = 103;
    matrix.graphmatrix[42][39].weight = matrix.graphmatrix[39][42].weight = 173;
    matrix.graphmatrix[42][41].weight = matrix.graphmatrix[41][42].weight = 84;
    matrix.graphmatrix[42][43].weight = matrix.graphmatrix[43][42].weight = 83;
    matrix.graphmatrix[42][50].weight = matrix.graphmatrix[50][42].weight = 105;
    matrix.graphmatrix[43][37].weight = matrix.graphmatrix[37][43].weight = 181;
    matrix.graphmatrix[43][42].weight = matrix.graphmatrix[42][43].weight = 83;
    matrix.graphmatrix[43][49].weight = matrix.graphmatrix[49][43].weight = 105;
    matrix.graphmatrix[43][105].weight = matrix.graphmatrix[105][43].weight = 91;
    matrix.graphmatrix[44][38].weight = matrix.graphmatrix[38][44].weight = 139;
    matrix.graphmatrix[44][45].weight = matrix.graphmatrix[45][44].weight = 206;
    matrix.graphmatrix[44][105].weight = matrix.graphmatrix[105][44].weight = 31;
    matrix.graphmatrix[45][44].weight = matrix.graphmatrix[44][45].weight = 206;
    matrix.graphmatrix[45][46].weight = matrix.graphmatrix[46][45].weight = 64;
    matrix.graphmatrix[46][33].weight = matrix.graphmatrix[33][46].weight = 147;
    matrix.graphmatrix[46][45].weight = matrix.graphmatrix[45][46].weight = 64;
    matrix.graphmatrix[46][48].weight = matrix.graphmatrix[48][46].weight = 240;
    matrix.graphmatrix[47][32].weight = matrix.graphmatrix[32][47].weight = 320;
    matrix.graphmatrix[47][56].weight = matrix.graphmatrix[56][47].weight = 211;
    matrix.graphmatrix[47][57].weight = matrix.graphmatrix[57][47].weight = 251;
    matrix.graphmatrix[48][46].weight = matrix.graphmatrix[46][48].weight = 240;
    matrix.graphmatrix[48][49].weight = matrix.graphmatrix[49][48].weight = 79;
    matrix.graphmatrix[48][105].weight = matrix.graphmatrix[105][48].weight = 98;
    matrix.graphmatrix[49][43].weight = matrix.graphmatrix[43][49].weight = 105;
    matrix.graphmatrix[49][48].weight = matrix.graphmatrix[48][49].weight = 79;
    matrix.graphmatrix[49][50].weight = matrix.graphmatrix[50][49].weight = 83;
    matrix.graphmatrix[49][56].weight = matrix.graphmatrix[56][49].weight = 83;
    matrix.graphmatrix[50][42].weight = matrix.graphmatrix[42][50].weight = 105;
    matrix.graphmatrix[50][49].weight = matrix.graphmatrix[49][50].weight = 83;
    matrix.graphmatrix[50][51].weight = matrix.graphmatrix[51][50].weight = 84;
    matrix.graphmatrix[50][55].weight = matrix.graphmatrix[55][50].weight = 94;
    matrix.graphmatrix[51][41].weight = matrix.graphmatrix[41][51].weight = 103;
    matrix.graphmatrix[51][50].weight = matrix.graphmatrix[50][51].weight = 84;
    matrix.graphmatrix[51][52].weight = matrix.graphmatrix[52][51].weight = 64;
    matrix.graphmatrix[51][54].weight = matrix.graphmatrix[54][51].weight = 88;
    matrix.graphmatrix[52][51].weight = matrix.graphmatrix[51][52].weight = 64;
    matrix.graphmatrix[52][53].weight = matrix.graphmatrix[53][52].weight = 88;
    matrix.graphmatrix[53][52].weight = matrix.graphmatrix[52][53].weight = 88;
    matrix.graphmatrix[53][54].weight = matrix.graphmatrix[54][53].weight = 64;
    matrix.graphmatrix[53][60].weight = matrix.graphmatrix[60][53].weight = 118;
    matrix.graphmatrix[54][51].weight = matrix.graphmatrix[51][54].weight = 88;
    matrix.graphmatrix[54][53].weight = matrix.graphmatrix[53][54].weight = 64;
    matrix.graphmatrix[54][55].weight = matrix.graphmatrix[55][54].weight = 83;
    matrix.graphmatrix[54][59].weight = matrix.graphmatrix[59][54].weight = 130;
    matrix.graphmatrix[55][50].weight = matrix.graphmatrix[50][55].weight = 94;
    matrix.graphmatrix[55][54].weight = matrix.graphmatrix[54][55].weight = 83;
    matrix.graphmatrix[55][56].weight = matrix.graphmatrix[56][55].weight = 90;
    matrix.graphmatrix[55][58].weight = matrix.graphmatrix[58][55].weight = 125;
    matrix.graphmatrix[56][47].weight = matrix.graphmatrix[47][56].weight = 211;
    matrix.graphmatrix[56][49].weight = matrix.graphmatrix[49][56].weight = 83;
    matrix.graphmatrix[56][55].weight = matrix.graphmatrix[55][56].weight = 90;
    matrix.graphmatrix[56][57].weight = matrix.graphmatrix[57][56].weight = 136;
    matrix.graphmatrix[57][47].weight = matrix.graphmatrix[47][57].weight = 251;
    matrix.graphmatrix[57][56].weight = matrix.graphmatrix[56][57].weight = 136;
    matrix.graphmatrix[57][58].weight = matrix.graphmatrix[58][57].weight = 88;
    matrix.graphmatrix[57][71].weight = matrix.graphmatrix[71][57].weight = 141;
    matrix.graphmatrix[58][55].weight = matrix.graphmatrix[55][58].weight = 125;
    matrix.graphmatrix[58][57].weight = matrix.graphmatrix[57][58].weight = 88;
    matrix.graphmatrix[58][59].weight = matrix.graphmatrix[59][58].weight = 75;
    matrix.graphmatrix[59][54].weight = matrix.graphmatrix[54][59].weight = 130;
    matrix.graphmatrix[59][58].weight = matrix.graphmatrix[58][59].weight = 75;
    matrix.graphmatrix[59][60].weight = matrix.graphmatrix[60][59].weight = 69;
    matrix.graphmatrix[60][53].weight = matrix.graphmatrix[53][60].weight = 118;
    matrix.graphmatrix[60][59].weight = matrix.graphmatrix[59][60].weight = 69;
    matrix.graphmatrix[60][61].weight = matrix.graphmatrix[61][60].weight = 114;
    matrix.graphmatrix[61][60].weight = matrix.graphmatrix[60][61].weight = 114;
    matrix.graphmatrix[61][62].weight = matrix.graphmatrix[62][61].weight = 79;
    matrix.graphmatrix[62][61].weight = matrix.graphmatrix[61][62].weight = 79;
    matrix.graphmatrix[62][63].weight = matrix.graphmatrix[63][62].weight = 192;
    matrix.graphmatrix[62][67].weight = matrix.graphmatrix[67][62].weight = 162;
    matrix.graphmatrix[63][62].weight = matrix.graphmatrix[62][63].weight = 192;
    matrix.graphmatrix[63][64].weight = matrix.graphmatrix[64][63].weight = 232;
    matrix.graphmatrix[63][65].weight = matrix.graphmatrix[65][63].weight = 104;
    matrix.graphmatrix[64][40].weight = matrix.graphmatrix[40][64].weight = 417;
    matrix.graphmatrix[64][63].weight = matrix.graphmatrix[63][64].weight = 232;
    matrix.graphmatrix[65][63].weight = matrix.graphmatrix[63][65].weight = 104;
    matrix.graphmatrix[65][66].weight = matrix.graphmatrix[66][65].weight = 191;
    matrix.graphmatrix[66][65].weight = matrix.graphmatrix[65][66].weight = 191;
    matrix.graphmatrix[66][79].weight = matrix.graphmatrix[79][66].weight = 304;
    matrix.graphmatrix[67][62].weight = matrix.graphmatrix[62][67].weight = 162;
    matrix.graphmatrix[67][68].weight = matrix.graphmatrix[68][67].weight = 77;
    matrix.graphmatrix[68][67].weight = matrix.graphmatrix[67][68].weight = 77;
    matrix.graphmatrix[69][70].weight = matrix.graphmatrix[70][69].weight = 84;
    matrix.graphmatrix[70][69].weight = matrix.graphmatrix[69][70].weight = 84;
    matrix.graphmatrix[70][72].weight = matrix.graphmatrix[72][70].weight = 104;
    matrix.graphmatrix[70][74].weight = matrix.graphmatrix[74][70].weight = 141;
    matrix.graphmatrix[71][57].weight = matrix.graphmatrix[57][71].weight = 141;
    matrix.graphmatrix[71][72].weight = matrix.graphmatrix[72][71].weight = 80;
    matrix.graphmatrix[72][70].weight = matrix.graphmatrix[70][72].weight = 104;
    matrix.graphmatrix[72][71].weight = matrix.graphmatrix[71][72].weight = 80;
    matrix.graphmatrix[72][73].weight = matrix.graphmatrix[73][72].weight = 102;
    matrix.graphmatrix[73][72].weight = matrix.graphmatrix[72][73].weight = 102;
    matrix.graphmatrix[73][74].weight = matrix.graphmatrix[74][73].weight = 158;
    matrix.graphmatrix[73][75].weight = matrix.graphmatrix[75][73].weight = 154;
    matrix.graphmatrix[74][70].weight = matrix.graphmatrix[70][74].weight = 141;
    matrix.graphmatrix[74][73].weight = matrix.graphmatrix[73][74].weight = 158;
    matrix.graphmatrix[74][78].weight = matrix.graphmatrix[78][74].weight = 172;
    matrix.graphmatrix[74][80].weight = matrix.graphmatrix[80][74].weight = 164;
    matrix.graphmatrix[75][73].weight = matrix.graphmatrix[73][75].weight = 154;
    matrix.graphmatrix[75][76].weight = matrix.graphmatrix[76][75].weight = 137;
    matrix.graphmatrix[76][75].weight = matrix.graphmatrix[75][76].weight = 137;
    matrix.graphmatrix[76][77].weight = matrix.graphmatrix[77][76].weight = 88;
    matrix.graphmatrix[77][76].weight = matrix.graphmatrix[76][77].weight = 88;
    matrix.graphmatrix[77][78].weight = matrix.graphmatrix[78][77].weight = 226;
    matrix.graphmatrix[77][84].weight = matrix.graphmatrix[84][77].weight = 120;
    matrix.graphmatrix[78][74].weight = matrix.graphmatrix[74][78].weight = 172;
    matrix.graphmatrix[78][77].weight = matrix.graphmatrix[77][78].weight = 226;
    matrix.graphmatrix[78][79].weight = matrix.graphmatrix[79][78].weight = 165;
    matrix.graphmatrix[79][66].weight = matrix.graphmatrix[66][79].weight = 304;
    matrix.graphmatrix[79][78].weight = matrix.graphmatrix[78][79].weight = 165;
    matrix.graphmatrix[79][80].weight = matrix.graphmatrix[80][79].weight = 51;
    matrix.graphmatrix[79][82].weight = matrix.graphmatrix[82][79].weight = 139;
    matrix.graphmatrix[80][74].weight = matrix.graphmatrix[74][80].weight = 164;
    matrix.graphmatrix[80][79].weight = matrix.graphmatrix[79][80].weight = 51;
    matrix.graphmatrix[80][81].weight = matrix.graphmatrix[81][80].weight = 119;
    matrix.graphmatrix[81][80].weight = matrix.graphmatrix[80][81].weight = 119;
    matrix.graphmatrix[82][0].weight = matrix.graphmatrix[0][82].weight = 128;
    matrix.graphmatrix[82][79].weight = matrix.graphmatrix[79][82].weight = 139;
    matrix.graphmatrix[82][83].weight = matrix.graphmatrix[83][82].weight = 415;
    matrix.graphmatrix[83][82].weight = matrix.graphmatrix[82][83].weight = 415;
    matrix.graphmatrix[84][77].weight = matrix.graphmatrix[77][84].weight = 120;
    matrix.graphmatrix[84][85].weight = matrix.graphmatrix[85][84].weight = 265;
    matrix.graphmatrix[85][8].weight = matrix.graphmatrix[8][85].weight = 186;
    matrix.graphmatrix[85][84].weight = matrix.graphmatrix[84][85].weight = 265;
    matrix.graphmatrix[86][16].weight = matrix.graphmatrix[16][86].weight = 68;
    matrix.graphmatrix[86][21].weight = matrix.graphmatrix[21][86].weight = 187;
    matrix.graphmatrix[86][87].weight = matrix.graphmatrix[87][86].weight = 64;
    matrix.graphmatrix[87][17].weight = matrix.graphmatrix[17][87].weight = 79;
    matrix.graphmatrix[87][20].weight = matrix.graphmatrix[20][87].weight = 187;
    matrix.graphmatrix[87][86].weight = matrix.graphmatrix[86][87].weight = 64;
    matrix.graphmatrix[88][17].weight = matrix.graphmatrix[17][88].weight = 613;
    matrix.graphmatrix[88][89].weight = matrix.graphmatrix[89][88].weight = 287;
    matrix.graphmatrix[89][88].weight = matrix.graphmatrix[88][89].weight = 287;
    matrix.graphmatrix[89][90].weight = matrix.graphmatrix[90][89].weight = 113;
    matrix.graphmatrix[89][102].weight = matrix.graphmatrix[102][89].weight = 689;
    matrix.graphmatrix[90][89].weight = matrix.graphmatrix[89][90].weight = 113;
    matrix.graphmatrix[90][91].weight = matrix.graphmatrix[91][90].weight = 191;
    matrix.graphmatrix[90][96].weight = matrix.graphmatrix[96][90].weight = 236;
    matrix.graphmatrix[91][90].weight = matrix.graphmatrix[90][91].weight = 191;
    matrix.graphmatrix[91][92].weight = matrix.graphmatrix[92][91].weight = 174;
    matrix.graphmatrix[91][95].weight = matrix.graphmatrix[95][91].weight = 179;
    matrix.graphmatrix[92][91].weight = matrix.graphmatrix[91][92].weight = 174;
    matrix.graphmatrix[92][93].weight = matrix.graphmatrix[93][92].weight = 216;
    matrix.graphmatrix[92][94].weight = matrix.graphmatrix[94][92].weight = 159;
    matrix.graphmatrix[93][92].weight = matrix.graphmatrix[92][93].weight = 216;
    matrix.graphmatrix[93][94].weight = matrix.graphmatrix[94][93].weight = 160;
    matrix.graphmatrix[93][99].weight = matrix.graphmatrix[99][93].weight = 192;
    matrix.graphmatrix[94][92].weight = matrix.graphmatrix[92][94].weight = 159;
    matrix.graphmatrix[94][93].weight = matrix.graphmatrix[93][94].weight = 160;
    matrix.graphmatrix[94][95].weight = matrix.graphmatrix[95][94].weight = 98;
    matrix.graphmatrix[94][98].weight = matrix.graphmatrix[98][94].weight = 177;
    matrix.graphmatrix[95][91].weight = matrix.graphmatrix[91][95].weight = 179;
    matrix.graphmatrix[95][94].weight = matrix.graphmatrix[94][95].weight = 98;
    matrix.graphmatrix[95][96].weight = matrix.graphmatrix[96][95].weight = 147;
    matrix.graphmatrix[95][97].weight = matrix.graphmatrix[97][95].weight = 178;
    matrix.graphmatrix[96][90].weight = matrix.graphmatrix[90][96].weight = 236;
    matrix.graphmatrix[96][95].weight = matrix.graphmatrix[95][96].weight = 147;
    matrix.graphmatrix[96][100].weight = matrix.graphmatrix[100][96].weight = 192;
    matrix.graphmatrix[97][95].weight = matrix.graphmatrix[95][97].weight = 178;
    matrix.graphmatrix[97][98].weight = matrix.graphmatrix[98][97].weight = 89;
    matrix.graphmatrix[97][100].weight = matrix.graphmatrix[100][97].weight = 51;
    matrix.graphmatrix[98][94].weight = matrix.graphmatrix[94][98].weight = 177;
    matrix.graphmatrix[98][97].weight = matrix.graphmatrix[97][98].weight = 89;
    matrix.graphmatrix[98][99].weight = matrix.graphmatrix[99][98].weight = 170;
    matrix.graphmatrix[98][101].weight = matrix.graphmatrix[101][98].weight = 190;
    matrix.graphmatrix[99][93].weight = matrix.graphmatrix[93][99].weight = 192;
    matrix.graphmatrix[99][98].weight = matrix.graphmatrix[98][99].weight = 170;
    matrix.graphmatrix[99][104].weight = matrix.graphmatrix[104][99].weight = 417;
    matrix.graphmatrix[100][96].weight = matrix.graphmatrix[96][100].weight = 192;
    matrix.graphmatrix[100][97].weight = matrix.graphmatrix[97][100].weight = 51;
    matrix.graphmatrix[100][101].weight = matrix.graphmatrix[101][100].weight = 164;
    matrix.graphmatrix[101][98].weight = matrix.graphmatrix[98][101].weight = 190;
    matrix.graphmatrix[101][100].weight = matrix.graphmatrix[100][101].weight = 164;
    matrix.graphmatrix[101][102].weight = matrix.graphmatrix[102][101].weight = 103;
    matrix.graphmatrix[102][89].weight = matrix.graphmatrix[89][102].weight = 689;
    matrix.graphmatrix[102][101].weight = matrix.graphmatrix[101][102].weight = 103;
    matrix.graphmatrix[102][103].weight = matrix.graphmatrix[103][102].weight = 115;
    matrix.graphmatrix[103][102].weight = matrix.graphmatrix[102][103].weight = 115;
    matrix.graphmatrix[103][104].weight = matrix.graphmatrix[104][103].weight = 101;
    matrix.graphmatrix[104][99].weight = matrix.graphmatrix[99][104].weight = 417;
    matrix.graphmatrix[104][103].weight = matrix.graphmatrix[103][104].weight = 101;
    matrix.graphmatrix[104][106].weight = matrix.graphmatrix[106][104].weight = 73;
    matrix.graphmatrix[105][43].weight = matrix.graphmatrix[43][105].weight = 91;
    matrix.graphmatrix[105][44].weight = matrix.graphmatrix[44][105].weight = 31;
    matrix.graphmatrix[105][48].weight = matrix.graphmatrix[48][105].weight = 98;
    matrix.graphmatrix[106][6].weight = matrix.graphmatrix[6][106].weight = 98;
    matrix.graphmatrix[106][104].weight = matrix.graphmatrix[104][106].weight = 73;

}


// setting names of places and buttons on the main screen
void MainWindow::toolbar ()
{
    //declaring toolbar and comboboxes to be constructed on the main window
    QToolBar *toolBar = addToolBar ("Tool");
    fromlabel = new QLabel(tr("Starting Point: "));
    startpoint = new QComboBox;

    // adding name of all points on the map in the comboboxes

    startpoint->addItem (tr("La'Appart Fitness"));  //1
    startpoint->addItem (tr("Safron Le Creusot"));  //2
    startpoint->addItem (tr("Hotel Le petite Verrerie"));  //3
    startpoint->addItem (tr("Chemin de fer Creusot"));  //4
    startpoint->addItem (tr("Brasserie De La Gare"));  //5
    startpoint->addItem (tr("Gare du Creusot")); //6
    startpoint->addItem (tr("Fish Shop"));  //7
    startpoint->addItem (tr("Carrefour"));  //8
    startpoint->addItem (tr("Tabac A cote de Carrefour"));  //9
    startpoint->addItem (tr("Industeel France Group"));    //10
    startpoint->addItem (tr("Rue Clemenceau"));    //11
    startpoint->addItem (tr("Rue Jean Bouveri"));    //12
    startpoint->addItem (tr("Rue de l'Etang de la forge")); //13
    startpoint->addItem (tr("Parking Industeel")); //14
    startpoint->addItem (tr("Etang du Parc")); //15
    startpoint->addItem (tr("Pompes")); //16
    startpoint->addItem (tr("Rue de Puebla"));   //17
    startpoint->addItem (tr("Rue de Sebastopol"));   //18
    startpoint->addItem (tr("Rue de Solferino"));   //19
    startpoint->addItem (tr("Rue des Acacias"));   //20
    startpoint->addItem (tr("Pharmacy near Aldi"));   //21
    startpoint->addItem (tr("Rue de Sebastopol"));   //22
    startpoint->addItem (tr("Rue de Solferino"));       //23
    startpoint->addItem (tr("Gym"));     //24
    startpoint->addItem (tr("Gourdiche Pneus 71"));     //25
    startpoint->addItem (tr("Aldi"));//26
    startpoint->addItem (tr("Alee du Parc"));  //27
    startpoint->addItem (tr("Acacias"));  //28
    startpoint->addItem (tr("Boulangerie Patisserie Boudot"));  //29
    startpoint->addItem (tr("Garage Godek"));  //30
    startpoint->addItem (tr("Entre elles"));  //31
    startpoint->addItem (tr("AMG Informatique 71"));  //32
    startpoint->addItem (tr("Residence Acacias")); //33
    startpoint->addItem (tr("Laundry"));  //34
    startpoint->addItem (tr("Rue Bourdon"));  //35
    startpoint->addItem (tr("Saint Henri"));  //36
    startpoint->addItem (tr("Parc de Carriers"));    //37
    startpoint->addItem (tr("Rue de Mulhouse"));    //38
    startpoint->addItem (tr("Rue Felix Martin"));    //39
    startpoint->addItem (tr("Nevers Stop Bus")); //40
    startpoint->addItem (tr("Church Eglise")); //41
    startpoint->addItem (tr("Rue de Marsaille")); //42
    startpoint->addItem (tr("Rue Lamartine")); //43
    startpoint->addItem (tr("Rue felix"));   //44
    startpoint->addItem (tr("Rue de Charolles"));   //45
    startpoint->addItem (tr("Church"));   //46
    startpoint->addItem (tr("Blondel Antonine"));   //47
    startpoint->addItem (tr("El Loco"));   //48
    startpoint->addItem (tr("collin"));   //49
    startpoint->addItem (tr("Skintatoo"));       //50
    startpoint->addItem (tr("Rue de blanzy"));     //51
    startpoint->addItem (tr("Collin Frederic"));     //52
    startpoint->addItem (tr("Rue Jean Baptiste Dumay"));//53
    startpoint->addItem (tr("Rue de Mollins"));  //54
    startpoint->addItem (tr("Fleurentin David"));  //55
    startpoint->addItem (tr("Rue Martine"));  //56
    startpoint->addItem (tr("Le Gardenia"));  //57
    startpoint->addItem (tr("Mediatheque"));  //58
    startpoint->addItem (tr("Tabac du Parc"));  //59
    startpoint->addItem (tr("Pharmacy")); //60
    startpoint->addItem (tr("La Fut-met"));  //61
    startpoint->addItem (tr("Dacosta"));  //62
    startpoint->addItem (tr("Foyer Personnes"));  //63
    startpoint->addItem (tr("Cycles 3C"));    //64
    startpoint->addItem (tr("Noz"));    //65
    startpoint->addItem (tr("Soleil"));    //66
    startpoint->addItem (tr("Le Gaulois")); //67
    startpoint->addItem (tr("Ecoles Maternelles")); //68
    startpoint->addItem (tr("ecole Marie curie")); //69
    startpoint->addItem (tr("Rue du Putis")); //70
    startpoint->addItem (tr("CIBC 71"));   //71
    startpoint->addItem (tr("Le Sabb Thai Association"));   //72
    startpoint->addItem (tr("Aurel Au Carre"));   //73
    startpoint->addItem (tr("Le Creusot Hotel"));   //74
    startpoint->addItem (tr("La Bonne Cave"));   //75
    startpoint->addItem (tr("Le Creusot Hotel"));   //76
    startpoint->addItem (tr("La Belle Epoque"));       //77
    startpoint->addItem (tr("Optician"));     //78
    startpoint->addItem (tr("LCL"));     //79
    startpoint->addItem (tr("Royal Tacos"));//80
    startpoint->addItem (tr("Highland Pub"));  //81
    startpoint->addItem (tr("Ecole Maternelle de La Molette"));  //82
    startpoint->addItem (tr("Halle des sports"));  //83
    startpoint->addItem (tr("Doctor Miralles Dominique"));  //84
    startpoint->addItem (tr("Chatue de La Varrerie"));  //85
    startpoint->addItem (tr("Hotel la Petit Verrarie"));  //86
    startpoint->addItem (tr("sst btp 71")); //87
    startpoint->addItem (tr("Bar le Magenta Baily"));  //88
    startpoint->addItem (tr("Leader Price"));  //89
    startpoint->addItem (tr("Cpam "));  //90
    startpoint->addItem (tr("Le cat et fils"));    //91
    startpoint->addItem (tr("Foyer Residence Personnes Agees"));    //92
    startpoint->addItem (tr("Proman Le Creusot"));    //93
    startpoint->addItem (tr("La Maison Dautel")); //94
    startpoint->addItem (tr("BNP")); //95
    startpoint->addItem (tr("jyp repro")); //96
    startpoint->addItem (tr("Sportonic Le Creusot")); //97
    startpoint->addItem (tr("Femme de Caractere"));   //98
    startpoint->addItem (tr("SFR"));   //99
    startpoint->addItem (tr("Docteure Gullemot"));   //100
    startpoint->addItem (tr("La Conca"));   //101
    startpoint->addItem (tr("CAF"));   //102
    startpoint->addItem (tr("Framatome"));   //103
    startpoint->addItem (tr("Au cochon ventru"));       //104
    startpoint->addItem (tr("Hotel 19 cent"));     //105
    startpoint->addItem (tr("Rue de charllos"));     //106
    startpoint->addItem (tr("Lassurance Maladie"));  //107


    Tolabel = new QLabel(tr("Finishing Point: "));

    endpoint = new QComboBox;
    endpoint->addItem (tr("La'Appart Fitness"));  //1
    /*-->addItem (tr("Safron Le Creusot"));  //2*/
    endpoint->addItem (tr("Hotel Le petite Verrerie"));  //3
    endpoint->addItem (tr("Chemin de fer Creusot"));  //4
    endpoint->addItem (tr("Brasserie De La Gare"));  //5
    endpoint->addItem (tr("Gare du Creusot")); //6
    endpoint->addItem (tr("Fish Shop"));  //7
    endpoint->addItem (tr("Carrefour"));  //8
    endpoint->addItem (tr("Tabac A cote de Carrefour"));  //9
    endpoint->addItem (tr("Industeel France Group"));    //10
    endpoint->addItem (tr("Rue Clemenceau"));    //11
    endpoint->addItem (tr("Rue Jean Bouveri"));    //12
    endpoint->addItem (tr("Rue de l'Etang de la forge")); //13
    endpoint->addItem (tr("Parking Industeel")); //14
    endpoint->addItem (tr("Etang du Parc")); //15
    endpoint->addItem (tr("Pompes")); //16
    endpoint->addItem (tr("Rue de Puebla"));   //17
    endpoint->addItem (tr("Rue de Sebastopol"));   //18
    endpoint->addItem (tr("Rue de Solferino"));   //19
    endpoint->addItem (tr("Rue des Acacias"));   //20
    endpoint->addItem (tr("Pharmacy near Aldi"));   //21
    endpoint->addItem (tr("Rue de Sebastopol"));   //22
    endpoint->addItem (tr("Rue de Solferino"));       //23
    endpoint->addItem (tr("Gym"));     //24
    endpoint->addItem (tr("Gourdiche Pneus 71"));     //25
    endpoint->addItem (tr("Aldi"));//26
    endpoint->addItem (tr("Alee du Parc"));  //27
    endpoint->addItem (tr("Acacias"));  //28
    endpoint->addItem (tr("Boulangerie Patisserie Boudot"));  //29
    endpoint->addItem (tr("Garage Godek"));  //30
    endpoint->addItem (tr("Entre elles"));  //31
    endpoint->addItem (tr("AMG Informatique 71"));  //32
    endpoint->addItem (tr("Residence Acacias")); //33
    endpoint->addItem (tr("Laundry"));  //34
    endpoint->addItem (tr("Rue Bourdon"));  //35
    endpoint->addItem (tr("Saint Henri"));  //36
    endpoint->addItem (tr("Parc de Carriers"));    //37
    endpoint->addItem (tr("Rue de Mulhouse"));    //38
    endpoint->addItem (tr("Rue Felix Martin"));    //39
    endpoint->addItem (tr("Nevers Stop Bus")); //40
    endpoint->addItem (tr("Church Eglise")); //41
    endpoint->addItem (tr("Rue de Marsaille")); //42
    endpoint->addItem (tr("Rue Lamartine")); //43
    endpoint->addItem (tr("Rue felix"));   //44
    endpoint->addItem (tr("Rue de Charolles"));   //45
    endpoint->addItem (tr("Church"));   //46
    endpoint->addItem (tr("Blondel Antonine"));   //47
    endpoint->addItem (tr("El Loco"));   //48
    endpoint->addItem (tr("collin"));   //49
    endpoint->addItem (tr("Skintatoo"));       //50
    endpoint->addItem (tr("Rue de blanzy"));     //51
    endpoint->addItem (tr("Collin Frederic"));     //52
   endpoint->addItem (tr("Rue Jean Baptiste Dumay"));//53
    endpoint->addItem (tr("Rue de Mollins"));  //54
    endpoint->addItem (tr("Fleurentin David"));  //55
    endpoint->addItem (tr("Rue Martine"));  //56
    endpoint->addItem (tr("Le Gardenia"));  //57
    endpoint->addItem (tr("Mediatheque"));  //58
    endpoint->addItem (tr("Tabac du Parc"));  //59
    endpoint->addItem (tr("Pharmacy")); //60
    endpoint->addItem (tr("La Fut-met"));  //61
    endpoint->addItem (tr("Dacosta"));  //62
    endpoint->addItem (tr("Foyer Personnes"));  //63
    endpoint->addItem (tr("Cycles 3C"));    //64
    endpoint->addItem (tr("Noz"));    //65
    endpoint->addItem (tr("Soleil"));    //66
    endpoint->addItem (tr("Le Gaulois")); //67
    endpoint->addItem (tr("Ecoles Maternelles")); //68
   endpoint->addItem (tr("ecole Marie curie")); //69
    endpoint->addItem (tr("Rue du Putis")); //70
    endpoint->addItem (tr("CIBC 71"));   //71
    endpoint->addItem (tr("Le Sabb Thai Association"));   //72
    endpoint->addItem (tr("Aurel Au Carre"));   //73
    endpoint->addItem (tr("Le Creusot Hotel"));   //74
    endpoint->addItem (tr("La Bonne Cave"));   //75
    endpoint->addItem (tr("Le Creusot Hotel"));   //76
    endpoint->addItem (tr("La Belle Epoque"));       //77
    endpoint->addItem (tr("Optician"));     //78
    endpoint->addItem (tr("LCL"));     //79
    endpoint->addItem (tr("Royal Tacos"));//80
    endpoint->addItem (tr("Highland Pub"));  //81
    endpoint->addItem (tr("Ecole Maternelle de La Molette"));  //82
    endpoint->addItem (tr("Halle des sports"));  //83
    endpoint->addItem (tr("Doctor Miralles Dominique"));  //84
    endpoint->addItem (tr("Chatue de La Varrerie"));  //85
    endpoint->addItem (tr("Hotel la Petit Verrarie"));  //86
    endpoint->addItem (tr("sst btp 71")); //87
    endpoint->addItem (tr("Bar le Magenta Baily"));  //88
    endpoint->addItem (tr("Leader Price"));  //89
    endpoint->addItem (tr("Cpam "));  //90
    endpoint->addItem (tr("Le cat et fils"));    //91
    endpoint->addItem (tr("Foyer Residence Personnes Agees"));    //92
    endpoint->addItem (tr("Proman Le Creusot"));    //93
    endpoint->addItem (tr("La Maison Dautel")); //94
    endpoint->addItem (tr("BNP")); //95
   endpoint->addItem (tr("jyp repro")); //96
    endpoint->addItem (tr("Sportonic Le Creusot")); //97
    endpoint->addItem (tr("Femme de Caractere"));   //98
    endpoint->addItem (tr("SFR"));   //99
    endpoint->addItem (tr("Docteure Gullemot"));   //100
    endpoint->addItem (tr("La Conca"));   //101
    endpoint->addItem (tr("CAF"));   //102
    endpoint->addItem (tr("Framatome"));   //103
    endpoint->addItem (tr("Au cochon ventru"));       //104
    endpoint->addItem (tr("Hotel 19 cent"));     //105
    endpoint->addItem (tr("Rue de charllos"));     //106
    endpoint->addItem (tr("Lassurance Maladie"));  //107



//these signals sent carry all data from combobox like index,string etc which can be chosen later in the function as per use
//but we will only require the index
    connect (startpoint, SIGNAL(activated(int)), this, SLOT(startcoordinates())); // signal from startpoint sent to startcoordinates()
    connect (endpoint, SIGNAL(activated(int)), this, SLOT(endcoordinates()));     // signal from startpoint sent to endcoordinates()

    //this is just for displaying and adding widgets on the main window for the user interface

    toolBar->addWidget (fromlabel);
    toolBar->addWidget (startpoint);
    toolBar->addSeparator ();
    toolBar->addWidget (Tolabel);
    toolBar->addWidget (endpoint);
    toolBar->addSeparator ();
    toolBar->addAction(buttonpresssearch);
    toolBar->addSeparator ();
    toolBar->addAction(buttonpressclear);

}



// this function takes the index of "starting point" drop down menu on the main window as input to case to initialise
// startXY X,Y points on the map
void MainWindow::startcoordinates ()
{
    switch (startpoint->currentIndex ())
    {
    case 0: startXY(560, 110); break;
    case 1: startXY(894, 95); break;
    case 2: startXY(900, 180); break;
    case 3: startXY(1076, 87); break;
    case 4: startXY(1084, 174); break;
    case 5: startXY(1148, 116); break;
    case 6: startXY(1201, 190); break;
    case 7: startXY(1144, 223); break;
    case 8: startXY(1076, 214); break;
    case 9: startXY(1044, 376); break;
    case 10: startXY(1124, 338); break;
    case 11: startXY(1091, 365); break;
    case 12: startXY(1059, 403); break;
    case 13: startXY(1140, 517); break;
    case 14: startXY(1156, 624); break;
    case 15: startXY(1198, 675); break;
    case 16: startXY(1253, 739); break;
    case 17: startXY(1301, 841); break;
    case 18: startXY(1212, 880); break;
    case 19: startXY(1126, 918); break;
    case 20: startXY(1191, 845); break;
    case 21: startXY(1177, 813); break;
    case 22: startXY(1165, 786); break;
    case 23: startXY(1084, 817); break;
    case 24: startXY(1095, 848); break;
    case 25: startXY(1110, 880); break;
    case 26: startXY(1132, 681); break;
    case 27: startXY(1013, 787); break;
    case 28: startXY(955, 877); break;
    case 29: startXY(999, 974); break;
    case 30: startXY(894, 902); break;
    case 31: startXY(916, 978); break;
    case 32: startXY(907, 755); break;
    case 33: startXY(826, 804); break;
    case 34: startXY(623, 871); break;
    case 35: startXY(628, 848); break;
    case 36: startXY(561, 833); break;
    case 37: startXY(577, 803); break;
    case 38: startXY(634, 819); break;
    case 39: startXY(534, 783); break;
    case 40: startXY(513, 725); break;
    case 41: startXY(529, 679); break;
    case 42: startXY(570, 698); break;
    case 43: startXY(612, 713); break;
    case 44: startXY(653, 747); break;
    case 45: startXY(763, 755); break;
    case 46: startXY(790, 734); break;
    case 47: startXY(760, 667); break;
    case 48: startXY(673, 681); break;
    case 49: startXY(635, 662); break;
    case 50: startXY(593, 647); break;
    case 51: startXY(552, 629); break;
    case 52: startXY(521, 615); break;
    case 53: startXY(540, 572); break;
    case 54: startXY(571, 586); break;
    case 55: startXY(613, 601); break;
    case 56: startXY(656, 623); break;
    case 57: startXY(684, 556); break;
    case 58: startXY(640, 540); break;
    case 59: startXY(603, 524); break;
    case 60: startXY(567, 515); break;
    case 61: startXY(514, 485); break;
    case 62: startXY(476, 466); break;
    case 63: startXY(374, 455); break;
    case 64: startXY(347, 576); break;
    case 65: startXY(360, 401); break;
    case 66: startXY(422, 320); break;
    case 67: startXY(541, 409); break;
    case 68: startXY(576, 387); break;
    case 69: startXY(585, 404); break;
    case 70: startXY(629, 414); break;
    case 71: startXY(647, 490); break;
    case 72: startXY(669, 453); break;
    case 73: startXY(697, 406); break;
    case 74: startXY(644, 340); break;
    case 75: startXY(706, 324); break;
    case 76: startXY(775, 300); break;
    case 77: startXY(779, 253); break;
    case 78: startXY(658, 249); break;
    case 79: startXY(570, 252); break;
    case 80: startXY(587, 273); break;
    case 81: startXY(533, 307); break;
    case 82: startXY(566, 178); break;
    case 83: startXY(788, 179); break;
    case 84: startXY(843, 250); break;
    case 85: startXY(977, 203); break;
    case 86: startXY(1268, 772); break;
    case 87: startXY(1282, 803); break;
    case 88: startXY(1600, 706); break;
    case 89: startXY(1520, 575); break;
    case 90: startXY(1559, 529); break;
    case 91: startXY(1619, 446); break;
    case 92: startXY(1643, 356); break;
    case 93: startXY(1595, 251); break;
    case 94: startXY(1562, 330); break;
    case 95: startXY(1549, 381); break;
    case 96: startXY(1485, 427); break;
    case 97: startXY(1458, 353); break;
    case 98: startXY(1470, 307); break;
    case 99: startXY(1497, 220); break;
    case 100: startXY(1436, 337); break;
    case 101: startXY(1373, 276); break;
    case 102: startXY(1319, 266); break;
    case 103: startXY(1285, 215); break;
    case 104: startXY(1282, 161); break;
    case 105: startXY(657, 731); break;
    case 106: startXY(1243, 159); break;



    default:
        break;
    }
}



// this function takes the index of "finishing point" drop down menu on the main window
// as input case to initialise endXY X,Y points according to our picture
void MainWindow::endcoordinates ()
{
    switch (endpoint->currentIndex ())
    {
    case 0: endXY(560, 110); break;
    case 1: endXY(894, 95); break;
    case 2: endXY(900, 180); break;
    case 3: endXY(1076, 87); break;
    case 4: endXY(1084, 174); break;
    case 5: endXY(1148, 116); break;
    case 6: endXY(1201, 190); break;
    case 7: endXY(1144, 223); break;
    case 8: endXY(1076, 214); break;
    case 9: endXY(1044, 376); break;
    case 10: endXY(1124, 338); break;
    case 11: endXY(1091, 365); break;
    case 12: endXY(1059, 403); break;
    case 13: endXY(1140, 517); break;
    case 14: endXY(1156, 624); break;
    case 15: endXY(1198, 675); break;
    case 16: endXY(1253, 739); break;
    case 17: endXY(1301, 841); break;
    case 18: endXY(1212, 880); break;
    case 19: endXY(1126, 918); break;
    case 20: endXY(1191, 845); break;
    case 21: endXY(1177, 813); break;
    case 22: endXY(1165, 786); break;
    case 23: endXY(1084, 817); break;
    case 24: endXY(1095, 848); break;
    case 25: endXY(1110, 880); break;
    case 26: endXY(1132, 681); break;
    case 27: endXY(1013, 787); break;
    case 28: endXY(955, 877); break;
    case 29: endXY(999, 974); break;
    case 30: endXY(894, 902); break;
    case 31: endXY(916, 978); break;
    case 32: endXY(907, 755); break;
    case 33: endXY(826, 804); break;
    case 34: endXY(623, 871); break;
    case 35: endXY(628, 848); break;
    case 36: endXY(561, 833); break;
    case 37: endXY(577, 803); break;
    case 38: endXY(634, 819); break;
    case 39: endXY(534, 783); break;
    case 40: endXY(513, 725); break;
    case 41: endXY(529, 679); break;
    case 42: endXY(570, 698); break;
    case 43: endXY(612, 713); break;
    case 44: endXY(653, 747); break;
    case 45: endXY(763, 755); break;
    case 46: endXY(790, 734); break;
    case 47: endXY(760, 667); break;
    case 48: endXY(673, 681); break;
    case 49: endXY(635, 662); break;
    case 50: endXY(593, 647); break;
    case 51: endXY(552, 629); break;
    case 52: endXY(521, 615); break;
    case 53: endXY(540, 572); break;
    case 54: endXY(571, 586); break;
    case 55: endXY(613, 601); break;
    case 56: endXY(656, 623); break;
    case 57: endXY(684, 556); break;
    case 58: endXY(640, 540); break;
    case 59: endXY(603, 524); break;
    case 60: endXY(567, 515); break;
    case 61: endXY(514, 485); break;
    case 62: endXY(476, 466); break;
    case 63: endXY(374, 455); break;
    case 64: endXY(347, 576); break;
    case 65: endXY(360, 401); break;
    case 66: endXY(422, 320); break;
    case 67: endXY(541, 409); break;
    case 68: endXY(576, 387); break;
    case 69: endXY(585, 404); break;
    case 70: endXY(629, 414); break;
    case 71: endXY(647, 490); break;
    case 72: endXY(669, 453); break;
    case 73: endXY(697, 406); break;
    case 74: endXY(644, 340); break;
    case 75: endXY(706, 324); break;
    case 76: endXY(775, 300); break;
    case 77: endXY(779, 253); break;
    case 78: endXY(658, 249); break;
    case 79: endXY(570, 252); break;
    case 80: endXY(587, 273); break;
    case 81: endXY(533, 307); break;
    case 82: endXY(566, 178); break;
    case 83: endXY(788, 179); break;
    case 84: endXY(843, 250); break;
    case 85: endXY(977, 203); break;
    case 86: endXY(1268, 772); break;
    case 87: endXY(1282, 803); break;
    case 88: endXY(1600, 706); break;
    case 89: endXY(1520, 575); break;
    case 90: endXY(1559, 529); break;
    case 91: endXY(1619, 446); break;
    case 92: endXY(1643, 356); break;
    case 93: endXY(1595, 251); break;
    case 94: endXY(1562, 330); break;
    case 95: endXY(1549, 381); break;
    case 96: endXY(1485, 427); break;
    case 97: endXY(1458, 353); break;
    case 98: endXY(1470, 307); break;
    case 99: endXY(1497, 220); break;
    case 100: endXY(1436, 337); break;
    case 101: endXY(1373, 276); break;
    case 102: endXY(1319, 266); break;
    case 103: endXY(1285, 215); break;
    case 104: endXY(1282, 161); break;
    case 105: endXY(657, 731); break;
    case 106: endXY(1243, 159); break;

    default:
        break;
    }
}




//contains the same coordinates as above but the function has the index input to it that is the end of the shortest path found
void MainWindow::moveTOposition (int index)
{
    switch (index)
    {
    case 0: endXY(560, 110); break;
    case 1: endXY(894, 95); break;
    case 2: endXY(900, 180); break;
    case 3: endXY(1076, 87); break;
    case 4: endXY(1084, 174); break;
    case 5: endXY(1148, 116); break;
    case 6: endXY(1201, 190); break;
    case 7: endXY(1144, 223); break;
    case 8: endXY(1076, 214); break;
    case 9: endXY(1044, 376); break;
    case 10: endXY(1124, 338); break;
    case 11: endXY(1091, 365); break;
    case 12: endXY(1059, 403); break;
    case 13: endXY(1140, 517); break;
    case 14: endXY(1156, 624); break;
    case 15: endXY(1198, 675); break;
    case 16: endXY(1253, 739); break;
    case 17: endXY(1301, 841); break;
    case 18: endXY(1212, 880); break;
    case 19: endXY(1126, 918); break;
    case 20: endXY(1191, 845); break;
    case 21: endXY(1177, 813); break;
    case 22: endXY(1165, 786); break;
    case 23: endXY(1084, 817); break;
    case 24: endXY(1095, 848); break;
    case 25: endXY(1110, 880); break;
    case 26: endXY(1132, 681); break;
    case 27: endXY(1013, 787); break;
    case 28: endXY(955, 877); break;
    case 29: endXY(999, 974); break;
    case 30: endXY(894, 902); break;
    case 31: endXY(916, 978); break;
    case 32: endXY(907, 755); break;
    case 33: endXY(826, 804); break;
    case 34: endXY(623, 871); break;
    case 35: endXY(628, 848); break;
    case 36: endXY(561, 833); break;
    case 37: endXY(577, 803); break;
    case 38: endXY(634, 819); break;
    case 39: endXY(534, 783); break;
    case 40: endXY(513, 725); break;
    case 41: endXY(529, 679); break;
    case 42: endXY(570, 698); break;
    case 43: endXY(612, 713); break;
    case 44: endXY(653, 747); break;
    case 45: endXY(763, 755); break;
    case 46: endXY(790, 734); break;
    case 47: endXY(760, 667); break;
    case 48: endXY(673, 681); break;
    case 49: endXY(635, 662); break;
    case 50: endXY(593, 647); break;
    case 51: endXY(552, 629); break;
    case 52: endXY(521, 615); break;
    case 53: endXY(540, 572); break;
    case 54: endXY(571, 586); break;
    case 55: endXY(613, 601); break;
    case 56: endXY(656, 623); break;
    case 57: endXY(684, 556); break;
    case 58: endXY(640, 540); break;
    case 59: endXY(603, 524); break;
    case 60: endXY(567, 515); break;
    case 61: endXY(514, 485); break;
    case 62: endXY(476, 466); break;
    case 63: endXY(374, 455); break;
    case 64: endXY(347, 576); break;
    case 65: endXY(360, 401); break;
    case 66: endXY(422, 320); break;
    case 67: endXY(541, 409); break;
    case 68: endXY(576, 387); break;
    case 69: endXY(585, 404); break;
    case 70: endXY(629, 414); break;
    case 71: endXY(647, 490); break;
    case 72: endXY(669, 453); break;
    case 73: endXY(697, 406); break;
    case 74: endXY(644, 340); break;
    case 75: endXY(706, 324); break;
    case 76: endXY(775, 300); break;
    case 77: endXY(779, 253); break;
    case 78: endXY(658, 249); break;
    case 79: endXY(570, 252); break;
    case 80: endXY(587, 273); break;
    case 81: endXY(533, 307); break;
    case 82: endXY(566, 178); break;
    case 83: endXY(788, 179); break;
    case 84: endXY(843, 250); break;
    case 85: endXY(977, 203); break;
    case 86: endXY(1268, 772); break;
    case 87: endXY(1282, 803); break;
    case 88: endXY(1600, 706); break;
    case 89: endXY(1520, 575); break;
    case 90: endXY(1559, 529); break;
    case 91: endXY(1619, 446); break;
    case 92: endXY(1643, 356); break;
    case 93: endXY(1595, 251); break;
    case 94: endXY(1562, 330); break;
    case 95: endXY(1549, 381); break;
    case 96: endXY(1485, 427); break;
    case 97: endXY(1458, 353); break;
    case 98: endXY(1470, 307); break;
    case 99: endXY(1497, 220); break;
    case 100: endXY(1436, 337); break;
    case 101: endXY(1373, 276); break;
    case 102: endXY(1319, 266); break;
    case 103: endXY(1285, 215); break;
    case 104: endXY(1282, 161); break;
    case 105: endXY(657, 731); break;
    case 106: endXY(1243, 159); break;


    default:
        break;
    }
}





