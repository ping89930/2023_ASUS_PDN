#ifndef FLOW_LP_H
#define FLOW_LP_H

#include <gurobi_c++.h>
#include "../base/Include.h"
#include "RGraph.h"
using namespace std;

class FlowLP {
    public:
        FlowLP(RGraph& rGraph, vector<double> vMediumLayerThickness, vector<double> vMetalLayerThickness, vector<double> vConductivity, double currentNorm);
        ~FlowLP() {}

        void setObjective(double areaWeight, double viaWeight);
        void setConserveConstraints();
        // width unit = meter
        void addCapacityConstraints(OASGEdge* e1, bool right1, double ratio1, OASGEdge* e2, bool right2, double ratio2, double width);
        void addCapacityConstraints(OASGEdge* e1, bool right1, double ratio1, double width);
        void relaxCapacityConstraints(GRBLinExpr& obj, OASGEdge* e1, bool right1, double ratio1, OASGEdge* e2, bool right2, double ratio2, double width);
        void solve();
        void collectResult();
        void printResult();

    private:
        RGraph& _rGraph;
        GRBEnv _env;
        GRBModel _model;
        // gurobi variables
        GRBVar*** _vPlaneLeftFlow;   // flows on the left of horizontal OASGEdges, index = [netId] [layId] [pEdgeId]
        GRBVar*** _vPlaneRightFlow;  // flows on the right of horizontal OASGEdges, index = [netId] [layId] [pEdgeId]
        GRBVar*** _vViaFlow;         // flows on vertical OASGEdges, index = [netId] [layPairId] [vEdgeId]
        GRBVar**  _vMaxViaCost;      // the maximum flow on an OASGEdge, index = [netId] [vEdgeId]

        // input constants
        vector<double> _vMediumLayerThickness;
        vector<double> _vMetalLayerThickness;
        vector<double> _vConductivity;
        double _currentNorm;      // the current value range (to avoid Gurobi error)
};

#endif