#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <set>
#include <cmath>

template <typename T>
std::string toString (const T val)
{
    std::stringstream ss;
    ss << val;
    return ss.str();
}

void replacementCriteriaMethod(std::vector<double> criteria, std::vector<std::vector<double>> mtrx, std::vector<double> optimal)
{
    double sum=0;
    std::vector <std::string> criteriaNames = {"A", "B", "C", "D"};
    std::cout << "Оригинальный вектор критерий\n";

    for (int i = 0; i < criteria.size(); i++) {
        sum = sum + criteria[i];
        std::cout << criteria[i] << "    ";
    }
    std::cout << "\nНормализованный вектор критерий\n";
    for (int i = 0; i < criteria.size(); i++) {
        criteria[i] = criteria[i] / sum;
        std::cout << criteria[i] << "    ";
    }

    std::cout << "\nМатрица оценок\n";
    for(int i = 0; i < mtrx.size(); i++)
    {
        std::cout << criteriaNames[i] << "    ";
        for (int j = 0; j < mtrx[0].size(); j++)
        {
            std::cout << mtrx[i][j] << "    ";
        }
        std::cout << "\n";
    }

    std::cout << "\nМатрица минимизации\n";
    for(int i = 0; i < mtrx.size(); i++)
        for (int j = 0; j < 2; j++)
            mtrx[i][j] = (10 - mtrx[i][j]);

    for(int i = 0; i < mtrx.size(); i++)
    {
        std::cout << criteriaNames[i] << "    ";
        for (int j = 0; j < mtrx[0].size(); j++)
        {
            std::cout << mtrx[i][j] << "    ";
        }
        std::cout << "\n";
    }

    int main_criteria = 3;

    for (int j = 0; j < mtrx[0].size(); j++)
    {
        if (j == (main_criteria-1))
            continue;
        double max = mtrx[0][j];
        double min = mtrx[0][j];
        for (int i = 0; i < mtrx.size(); i++)
        {
            if (mtrx[i][j] > max)
                max = mtrx[i][j];
            if (mtrx[i][j] < min)
                min = mtrx[i][j];
        }

        for (int i = 0; i < mtrx.size(); i++)
        {
            mtrx[i][j] = (mtrx[i][j] - min) / (max - min);
        }

    }
    std::cout << "\nНормализованная оценок\n";
    for(int i = 0; i < mtrx.size(); i++)
    {
        std::cout << criteriaNames[i] << "    ";
        for (int j = 0; j < mtrx[0].size(); j++)
        {
            std::cout << std::fixed << std::setprecision(3) << mtrx[i][j] << "    ";
        }
        std::cout << "\n";
    }

    int solve = 0;
    std::vector<double> vmax; vmax.resize(4,0);

    for (int j = 0; j < mtrx[0].size(); j++)
    {
        if (j == (main_criteria-1))
            continue;
        vmax[j] = mtrx[0][j];
        for (int i = 0; i < mtrx.size(); i++)
        {
            if (mtrx[i][j] > vmax[j])
                vmax[j] = mtrx[i][j];
        }
    }

    for (int i = 0; i < mtrx.size(); i++)
    {
        if (mtrx[i][0] >= vmax[0]*optimal[0] && mtrx[i][1] >= vmax[1]*optimal[1] && mtrx[i][3] >= vmax[3]*optimal[3])
            solve = i;
    }

    std::cout << "Решение: " << criteriaNames[solve] << "\n";
}


void ParetoMethod(std::vector<std::vector<double>> mtrx, int x, int y)
{
    struct ParetoDot
    {
        std::string name;
        double x;
        double y;

        ParetoDot(): name(""), x(0), y(0) {}
        ParetoDot(std::string name, double x, double y)
        {
            this->name = name;
            this->x = x;
            this->y = y;
        }

        void print()
        {
            std::cout << name << "(" << x << " ," << y<<")\n";
        }

        double dist(ParetoDot B)
        {
            double tmp1 = abs (this->x - B.x);
            double tmp2 = abs (this->y - B.y);

            if (tmp2 > tmp1)
                return tmp2;
            return tmp1;
        }
    };

    std::vector <std::string> criteriaNames = {"A", "B", "C", "D"};
    std::cout << "\nМатрица оценок\n";
    for(int i = 0; i < mtrx.size(); i++)
    {
        std::cout << criteriaNames[i] << "    ";
        for (int j = 0; j < mtrx[0].size(); j++)
        {
            std::cout << mtrx[i][j] << "    ";
        }
        std::cout << "\n";
    }
    std::vector<ParetoDot> dots; dots.resize(4);

    std::cout << "\nПолученные точки\n";
    for(int i = 0; i < mtrx.size(); i++)
    {
        dots[i].name = criteriaNames[i];
        dots[i].x = mtrx[i][x];
        dots[i].y = mtrx[i][y];
        dots[i].print();
    }

    ParetoDot Utopia ("O", 10, 0);
    Utopia.print();

    std::vector<double> dist; dist.resize(4,0);
    double minDist = 11;
    for (int i = 0; i < dots.size(); i++)
    {
        dist[i] = Utopia.dist(dots[i]);
        if (dist[i] < minDist)
            minDist = dist[i];
    }

    std::cout << "\nТочки с минимальными расстоянием Чебышева до точки Утопии\n";
    for (int i = 0; i < dots.size(); i++)
    {
        if (minDist == dist[i]) dots[i].print();
    }
}

void weightsMethod (std::vector<std::vector<double>> mtrx, std::vector<double> criteria)
{
    double max = -1;
    double amount = 0;
    std::vector<double> result_vector(mtrx[0].size());
    std::vector<std::vector<int>> a = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    std::vector<double> weights (4);
    std::set<std::string> value;
    std::vector<double> sum(4);
    std::vector<std::string> criteriaNames = {"A", "B", "C", "D"};
    std::cout << "\nМатрица оценок\n";
    for (int i = 0; i < mtrx.size(); i++) {
        std::cout << criteriaNames[i] << "    ";
        for (int j = 0; j < mtrx[0].size(); j++) {
            std::cout << mtrx[i][j] << "    ";
        }
        std::cout << "\n";
    }

    std::cout << "\nМатрица минимизации\n";
    for (int i = 0; i < mtrx.size(); i++)
        for (int j = 0; j < 2; j++)
            mtrx[i][j] = (10 - mtrx[i][j]);

    for (int i = 0; i < mtrx.size(); i++) {
        std::cout << criteriaNames[i] << "    ";
        for (int j = 0; j < mtrx[0].size(); j++) {
            std::cout << mtrx[i][j] << "    ";
        }
        std::cout << "\n";
    }

    std::cout << "\nНормализованная матрица\n";
    for (int i = 0; i < mtrx.size(); i++) {
        for (int j = 0; j < mtrx[0].size(); j++)
            sum[i] = sum[i] + mtrx[j][i];
    }

    for (int i = 0; i < mtrx.size(); i++)
        for (int j = 0; j < mtrx[0].size(); j++)
            mtrx[j][i] = mtrx[j][i] / sum[i];

    for (int i = 0; i < mtrx.size(); i++) {
        std::cout << criteriaNames[i] << "    ";
        for (int j = 0; j < mtrx[0].size(); j++) {
            std::cout << std::fixed << std::setprecision(3) << mtrx[i][j] << "    ";
        }
        std::cout << "\n";
    }
    for (int i = 0; i < criteria.size() ; i++)
    {
        for (int j = 0; j < criteria.size(); j++)
        {
            if(i != j)
            {
                if (criteria[i] > criteria[j]) {
                    a[i][j] = 1;
                    std::cout << "y" + toString(i + 1) + toString(j + 1) + ": " << 1 << std::endl;
                } else if (criteria[i] == criteria[j]) {
                    a[i][j] = 0.5;
                    std::cout << "y" + toString(i + 1) + toString(j + 1) + ": " << 0.5 << std::endl;
                } else {
                    a[i][j] = 0;
                }
            }
        }
    }
    for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < a[0].size(); j++)
        {
            weights[i] = weights[i] + a[i][j];
        }
    }
    std::cout << "Вектор критериев: " << std::endl;

    std::cout << "a = (";
    for (int i = 0; i < weights.size(); i++)
    {
        std::cout << weights[i] << ",";
    }
    std::cout << ")" << std::endl;

    std::cout << "Нормализованный вектор критериев: " << std::endl;

    for (int i = 0; i < weights.size(); i++)
    {
        amount = amount + weights[i];
    }

    std::cout << "a = (";
    for (int i = 0; i < weights.size(); i++)
    {
        weights[i] = weights[i]/amount;
        std::cout << weights[i] << ",";
    }
    std::cout << ")" << std::endl;

    for(int i = 0; i < mtrx.size(); i++)
    {
        result_vector[i]=0;
        for(int j=0; j<mtrx[0].size(); j++)
        {
            result_vector[i]+=mtrx[i][j]*weights[j];
        }
    }

    int count = 0;
    std::cout << "Значения объединенного критерия для всех альтернатив: " << std::endl;
    for(int i=0; i < result_vector.size(); i++)
    {
        if(max < result_vector[i])
        {
            max = result_vector[i];
            count = i;
        }
        std::cout << result_vector[i] << std::endl;
    }

    std::cout << "Наиболее приемлимая альтернатива: " << criteriaNames[count] << ", value = " << max << std::endl;
}

void Hierarchyanalysismethod(std::vector<std::vector<double>> mtrx, std::string parametr)
{
    double amount = 0;
    std::vector<double> sum_col(mtrx[0].size());
    std::vector <std::string> criteriaNames = {"A", "B", "C", "D"};
    if(parametr == "min")
    {
        for(int i = 0; i < mtrx.size(); i++)
        {
            for (int j = 0; j < mtrx[0].size(); j++)
            {
                mtrx[i][j] = std::pow(mtrx[i][j], -1);
            }
        }
    } else if(parametr == "max") {}else
    {
        std::cerr << "Wrong parameter" << std::endl;
        return;
    }

    std::cout << "Матрица оценок\n";
    for(int i = 0; i < mtrx.size(); i++)
    {
        std::cout << criteriaNames[i] << "    ";
        for (int j = 0; j < mtrx[0].size(); j++)
        {
            std::cout << mtrx[i][j] << "    ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";

    for(int i = 0; i < mtrx.size(); i++)
    {
        for (int j = 0; j < mtrx[0].size(); j++)
        {
            sum_col[i] = sum_col[i] + mtrx[i][j];
        }
        std::cout <<"Сумма по " << i+1 << "-ой " << "строке: " << sum_col[i] <<"\n";
    }

    for(int i = 0; i < mtrx.size(); i++)
    {
        for (int j = 0; j < mtrx[0].size(); j++)
        {
            amount = amount + mtrx[i][j];
        }
    }
    std::cout << "\n";

    for(int i = 0; i < sum_col.size(); i++)
    {
        std::cout << "Нормализованная cумма по " << i+1 << "-ой " << "строке: " << sum_col[i] / amount << std::endl;
    }
    std::cout << "\n";

}

std::vector<double> Hierarchyanalysismethod(std::vector<std::vector<double>> mtrx, bool parametr)
{
    double amount = 0;
    std::vector<double> sum_col(mtrx[0].size());
    std::vector <std::string> criteriaNames = {"A", "B", "C", "D"};
    std::vector<double> normSum; normSum.resize(mtrx.size());
    if(parametr)
    {
        for(int i = 0; i < mtrx.size(); i++)
        {
            for (int j = 0; j < mtrx[0].size(); j++)
            {
                mtrx[i][j] = std::pow(mtrx[i][j], -1);
            }
        }
    }

    std::cout << "Матрица оценок\n";
    for(int i = 0; i < mtrx.size(); i++)
    {
        std::cout << criteriaNames[i] << "    ";
        for (int j = 0; j < mtrx[0].size(); j++)
        {
            std::cout << mtrx[i][j] << "    ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";

    for(int i = 0; i < mtrx.size(); i++)
    {
        for (int j = 0; j < mtrx[0].size(); j++)
        {
            sum_col[i] = sum_col[i] + mtrx[i][j];
        }
        std::cout <<"Сумма по " << i+1 << "-ой " << "строке: " << sum_col[i] <<"\n";
    }

    for(int i = 0; i < mtrx.size(); i++)
    {
        for (int j = 0; j < mtrx[0].size(); j++)
        {
            amount = amount + mtrx[i][j];
        }
    }
    std::cout << "\n";

    for(int i = 0; i < sum_col.size(); i++)
    {
        normSum[i] = sum_col[i] / amount;
        std::cout << "Нормализованная cумма по " << i+1 << "-ой " << "строке: " <<  normSum[i] << std::endl;
    }
    std::cout << "\n";

    return  normSum;
}

double consistencyRelation (std::vector<std::vector<double>> mtrx)
{
    double result = 0;
    std::vector<double> mult; mult.resize(mtrx.size(),1);
    double sum = 0;
    for (int i = 0; i < mtrx.size(); i++)
    {
        for (int j = 0; j < mtrx[0].size(); j++)
        {
            mult[i]= mult[i] * mtrx[i][j];
        }
        mult[i] = sqrt(sqrt(mult[i]));
        sum = sum + mult[i];
    }
    std::vector<double> nvp; nvp.resize(mtrx.size(), 0);
    for (int i = 0; i < mtrx.size(); i++)
        nvp[i]=mult[i]/sum;
    double lambda = 0;
    for (int i = 0; i < mtrx.size(); i++)
    {
        double col_sum = 0;
        for (int j = 0; j < mtrx[0].size(); j++)
        {
            col_sum = col_sum + mtrx[j][i];
        }
        lambda = lambda + col_sum * nvp[i];

    }
    double index = (lambda - mtrx.size() ) / (mtrx.size() -1);

    return index / 0.9;

}

void Hierarchyanalysismethod2(std::vector<std::vector<double>> crit1, std::vector<std::vector<double>> crit2, std::vector<std::vector<double>> crit3, std::vector<std::vector<double>> crit4, std::vector<bool> param ,  std::vector<std::vector<double>> rating)
{
    std::vector <std::vector<std::vector<double>>> mtrx3d;
    mtrx3d.resize(4);
    mtrx3d[0]=crit1;
    mtrx3d[1]=crit2;
    mtrx3d[2]=crit3;
    mtrx3d[3]=crit4;

    double amount = 0;
    std::vector<std::vector<double>> sum_row;
    std::vector<std::vector<double>> mtrxNormSum;
    mtrxNormSum.resize(mtrx3d.size(), std::vector<double> (mtrx3d[0].size(), 0));
    sum_row.resize(mtrx3d.size(), std::vector<double>(mtrx3d[0].size(), 0));
    std::vector <std::string> criteriaNames = {"A", "B", "C", "D"};

    for (int m = 0; m < mtrx3d.size(); m++)
    {
        mtrxNormSum[m] = Hierarchyanalysismethod(mtrx3d[m],param[m]);
        std::cout << "Отношение согласованности: " << consistencyRelation (mtrx3d[m]) << "\n";


    }

    std::cout << "Вычисление ормализованной суммы по строке для матрицы оценки критериев\n";
    std::vector<double> ratingNorm = Hierarchyanalysismethod(rating, false);


    std::vector<double> matrxMult;
    matrxMult.resize(mtrx3d.size(), 0);
    for(int i = 0; i < mtrxNormSum.size(); i++)
    {

        for(int j=0; j<mtrx3d[0].size(); j++)
        {
            matrxMult[i]+=mtrxNormSum[j][i]*ratingNorm[i];
        }
    }

    int count = 0;
    double max = 0;

    std::cout << "Значения объединенного критерия для всех альтернатив: " << std::endl;
    for(int i=0; i < matrxMult.size(); i++)
    {
        if(max < matrxMult[i])
        {
            max = matrxMult[i];
            count = i;
        }
        std::cout << matrxMult[i] << std::endl;
    }

    std::cout << "Наиболее приемлимая альтернатива: " << criteriaNames[count] << ", value = " << max << std::endl;

}

int main()
{
    std::vector<double> criteria = {6,4,8,2};
    std::vector<std::vector<double>> mtrx = {{7,5,7,6},{7,6,7,5},{3,7,2,3},{5,8,2,2}};
    std::vector<double> optimal = {0.5, 0.3, 1, 0.1};

    replacementCriteriaMethod (criteria, mtrx, optimal);
    std::cout << "*****************************************" << std::endl;

    ParetoMethod(mtrx,2,0);
    std::cout << "*****************************************" << std::endl;

    weightsMethod(mtrx, criteria);
    std::cout << "*****************************************" << std::endl;

//    std::cout << "Первый критерий(Цена): " << std::endl;
    std::vector<std::vector<double>> crit1 = {{1,1,5,3},{1,1,5,3},{0.2,0.2,1,0.33333},{0.33333,0.33333,3,1}};
//    Hierarchyanalysismethod(crit1,"min");
//    std::cout << "Второй критерий(Стоимость обслуживания): " << std::endl;
    std::vector<std::vector<double>> crit2 = {{1,0.33333,0.2,0.14285714},{3,1,0.33333,0.2},{5,3,1,0.33333},{7,5,3,1}};
//    Hierarchyanalysismethod(crit2,"min");
//    std::cout << "Третий критерий(Память): " << std::endl;
    std::vector<std::vector<double>> crit3 = {{1,1,5,5},{1,1,5,5},{0.2,0.2,1,1},{0.2,0.2,1,1}};
//    Hierarchyanalysismethod(crit3,"max");
//    std::cout << "Четвертый критерий(Экран): " << std::endl;
    std::vector<std::vector<double>> crit4 = {{1,3,5,7},{0.33333,1,3,5},{0.2,0.33333,1,3},{0.2,0.33333,0.14285714,1}};
//    Hierarchyanalysismethod(crit4,"max");
    std::vector<std::vector<double>> rating = {{1,3,0.33333,5},{0.33333,1,0.2,3},{3,5,1,7},{0.2,0.33333,0.14285714,1}};
    Hierarchyanalysismethod2(crit1, crit2,crit3,crit4,{true, true, false, false}, rating);
    return 0;
}
