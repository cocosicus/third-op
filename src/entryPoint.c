#include "entryPoint.h"

#include "logic.h"

void doOperation(Operation operation, Context** context, Params* param) {
    switch (operation) {
    case CreateContext:
        createContext(context);
        break;
    case ClearContext:
        clearContext(*context);
        break;
    case LoadData:
        loadData(*context, param->params.loadDataParams.filename);
        break;
    case CalcStats:
        calculateStats(*context, param->params.calculateStatsParams.region, param->params.calculateStatsParams.column);
        break;
    default:
        break;
    }
}


