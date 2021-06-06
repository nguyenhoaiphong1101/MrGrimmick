#include "Grid.h"
#include "Utils.h"
void CGrid::Clear()
{
	for (int i = 0; i < ROW_MAX; i++)
	{
		for (int j = 0; j < COLUMN_MAX; j++)
		{
			cells[i][j].clear();
			for (UINT k = 0; k < cells[row][column].size(); k++)
			{
				delete (cells[row][column].at(k));
			}
		}
	}
}

void CGrid::Add(LPGAMEOBJECT Obj)
{
	int beginCellColumn, beginCellRow, endCellColumn, endCellRow;
	float l, t, r, b;
	Obj->GetBoundingBox(l, t, r, b);

	beginCellColumn = (int)(l / CELL_WIDTH);
	beginCellRow = (int)(t / CELL_HEIGHT);

	endCellColumn = (int)(r / CELL_WIDTH);
	endCellRow = (int)(b / CELL_HEIGHT);

	for (int i = beginCellRow; i <= endCellRow; i++)
	{
		for (int j = beginCellColumn; j <= endCellColumn; j++)
		{
			cells[i][j].push_back(Obj);
		}
	}


}

CGrid* CGrid::__instance = NULL;


CGrid* CGrid::GetInstance()
{
	if (__instance == NULL) __instance = new CGrid();
	return __instance;
}

void CGrid::GetUpdateObjects(vector<LPGAMEOBJECT>& updateobjects, float left, float top, float right, float bottom)
{
	int firstCellColumn, firstCellRow;		
	int lastCellColumn, lastCellRow;		

	firstCellColumn = (int)left / CELL_WIDTH;
	firstCellRow = (int)top / CELL_HEIGHT;

	lastCellColumn = (int)(right) / CELL_WIDTH;
	lastCellRow = (int)(bottom +100) / CELL_HEIGHT;


	for (int row = firstCellRow; row <= lastCellRow; row++)
	{
		for (int column = firstCellColumn; column <= lastCellColumn; column++)
		{
			for (UINT k = 0; k < cells[row][column].size(); k++)
			{
				updateobjects.push_back(cells[row][column].at(k));
			}
		}
	}
}

