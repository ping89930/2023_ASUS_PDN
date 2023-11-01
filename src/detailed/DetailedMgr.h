#ifndef DETAILED_MGR_H
#define DETAILED_MGR_H

#include "../base/Include.h"
#include "../base/DB.h"
#include "DetailedDB.h"
#include "AStarRouter.h"
#include <utility>
using namespace std;

class DetailedMgr {
    public:
        DetailedMgr(DB& db, SVGPlot& plot, double gridWidth) : _db(db), _plot(plot), _gridWidth(gridWidth) {
            _numXs = _db.boardWidth() / _gridWidth;
            _numYs = _db.boardHeight() / _gridWidth;
            for (size_t layId = 0; layId < _db.numLayers(); ++ layId) {
                vector< vector<Grid*> > vLayGrid;
                for (size_t xId = 0; xId < _numXs; ++ xId) {
                    vector<Grid*> vXGrid;
                    for (size_t yId = 0; yId < _numYs; ++ yId) {
                        Grid* grid = new Grid(xId, yId);
                        vXGrid.push_back(grid);
                    }
                    vLayGrid.push_back(vXGrid);
                }
                _vGrid.push_back(vLayGrid);
            }
            for (size_t netId = 0; netId < _db.numNets(); ++ netId) {
                vector< vector< Grid* > > vNetGrid;
                for (size_t layId = 0; layId < _db.numLayers(); ++ layId) {
                    vector<Grid*> vLayGrid;
                    vNetGrid.push_back(vLayGrid);
                }
                _vNetGrid.push_back(vNetGrid);
            }
        }
        ~DetailedMgr() {}

        void initGridMap();
        void plotGridMap();
        void naiveAStar();

        void print() {
            printf("\n===print=====\n");
            printf("_numXs: %d\n", _numXs);
            printf("_numYs: %d\n", _numYs);
            printf("vGrid.size(): %d\n", _vGrid.size());
            for(int i=0; i<_vGrid.size(); i++) {
                printf("vGrid[%d].size(): %d\n", i, _vGrid[i].size());
                for(int j=0; j<_vGrid[i].size(); j++) {
                    printf("vGrid[%d][%d].size(): %d\n", i, j, _vGrid[i][j].size());
                    for(int k=0; k<_vGrid[i][j].size(); k++) {
                        printf("vGrid[%d][%d][%d]: ", i, j, k);
                        _vGrid[i][j][k]->print();
                    }
                }
            }
            printf("\n\n");

            printf("vNetGrid.size(): %d\n", _vNetGrid.size());
            for(int i=0; i<_vNetGrid.size(); i++) {
                printf("vNetGrid[%d].size(): %d\n", i, _vNetGrid[i].size());
                for(int j=0; j<_vNetGrid[i].size(); j++) {
                    printf("vNetGrid[%d][%d].size(): %d\n", i, j, _vNetGrid[i][j].size());
                    for(int k=0; k<_vNetGrid[i][j].size(); k++) {
                        printf("vNetGrid[%d][%d][%d]: ", i, j, k);
                        _vNetGrid[i][j][k]->print();
                    }
                }
            }
        }
        void buildMtx();
        void writeColorMap(const char*, bool);

    private:
        void clearNet(size_t layId, size_t netId);
        bool legal(int xId, int yId) { return (xId>=0 && xId<_vGrid[0].size() && yId>=0 && yId<_vGrid[0][0].size()); }
        DB& _db;
        SVGPlot& _plot;
        double _gridWidth;
        vector< vector< vector< Grid* > > > _vGrid;     // index = [layId] [xId] [yId]; from left xId = 0, from bottom yId = 0
        vector< vector< vector< Grid* > > > _vNetGrid;  // index = [netId] [layId] [gridId]
        size_t _numXs;
        size_t _numYs;
};

#endif