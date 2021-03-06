#include "Curvature.h"
namespace dragon
{
namespace geometry
{
namespace mesh
{
    void ComputeCotanWeight(HalfEdge &he)
    {
        auto &edges = he.edges;
        he.ResetWeight();
        for(size_t i = 0; i != edges.size(); ++i)
        {
            if(edges[i]->twin_edge == nullptr) continue;
            else if(edges[i]->weight == 0)
            {
                const Point3 &v_last = edges[i]->twin_edge->next_edge->des_vertex->coor;
                const Point3 &v_next = edges[i]->next_edge->des_vertex->coor;
                const Point3 &v_current = edges[i]->des_vertex->coor;  ;
                const Point3 &v_ = edges[i]->ori_vertex->coor;  

                double cot_weight =  0.5 * (ClampCot(v_ - v_last, v_current - v_last) + ClampCot(v_ - v_next, v_current - v_next));
                cot_weight = std::max(0.05, cot_weight);
                edges[i]->weight = cot_weight;
                edges[i]->twin_edge->weight = cot_weight;
            }
        }
    }
    // compute mean curvature, which will also compute the local weights
    void ComputeMeanCurvature(HalfEdge &he, geometry::ScalarList &mean_curvatures)
    {
        mean_curvatures.clear();
        auto &vertices = he.vertices;
        // auto &faces = he.faces;
        //auto &edges = he.edges;
        auto &is_border = he.is_border;
        if(is_border.size() == 0)
        {
            std::cout<<RED<< "[ERROR]::[MeanCurvature]::You need to call CheckBorder firstly."<<RESET<<std::endl;
            return;
        }
        for(size_t i = 0; i != vertices.size(); ++i)
        {
            //compute mean Curvature
            // first step, find all 1-neighbor
            geometry::Vector3 mean_curvature_vector(0.0, 0.0, 0.0);
            double sum_area = 0.0;

            if(is_border[i])
            {
                mean_curvatures.push_back(0.0);
                continue;
            }
            auto current_edge =  vertices[i]->inc_edge;
            auto start_edge = vertices[i]->inc_edge;
            while(true)
            {
                if(current_edge == nullptr || current_edge->twin_edge == nullptr)
                {
                    std::cout<<YELLOW<<"[WARNING]::[MeanCurvature]::Not a model which can be embedded into a halfedge, maybe there are some  non-connected vertices."<<RESET<<std::endl;
                    break;
                }
                const Point3 &v_last =current_edge->twin_edge->next_edge->des_vertex->coor;
                const Point3 &v_next = current_edge->next_edge->des_vertex->coor;
                const Point3 &v_current = current_edge->des_vertex->coor;
                const Point3 &v_ = current_edge->ori_vertex->coor;  

                double cota = geometry::ClampCot(v_ - v_last, v_current - v_last);
                double cotb = geometry::ClampCot(v_ - v_next, v_current - v_next);
                double cot_weight =  0.5 * ( cota + cotb );
                current_edge->weight = cot_weight;             
                int  angle1 = geometry::AngleType(v_current - v_, v_next - v_), angle2 = geometry::AngleType(v_ - v_current, v_next - v_current), 
                    angle3 = geometry::AngleType(v_ - v_next, v_current - v_next);
                if(!std::isnan(cot_weight))
                {
                    if(angle1 > 0)
                        sum_area += 0.5 * geometry::ComputeTriangleArea(v_, v_current, v_next);
                    else if(angle2 == 0 && angle3 == 0)
                    sum_area += 0.125 * ((v_next - v_).squaredNorm() * geometry::ClampCot(v_ - v_current, v_next - v_current)
                     + (v_current - v_).squaredNorm() * cotb);
                    else  sum_area += 0.25 * geometry::ComputeTriangleArea(v_, v_current, v_next);

                    mean_curvature_vector += cot_weight  * (v_current - v_);
                }
                current_edge = current_edge->twin_edge->next_edge;
                if(current_edge == start_edge) break;

            }
            mean_curvatures.push_back( (mean_curvature_vector / sum_area / 3).norm());
        }
    }
    void ComputeGaussCurvature(HalfEdge &he, geometry::ScalarList &gauss_curvatures)
    {
        gauss_curvatures.clear();
        auto &vertices = he.vertices;
        // auto &faces = he.faces;
        //auto &edges = he.edges;
        auto &is_border = he.is_border;
        if(is_border.size() == 0)
        {
            std::cout<<RED<< "[ERROR]::[GaussCurvature]::You need to call CheckBorder firstly."<<RESET<<std::endl;
            return;
        }
        for(size_t i = 0; i != vertices.size(); ++i)
        {
            if(is_border[i])
            {
                gauss_curvatures.push_back(0);
                continue;
            }
            double sum_angle = 0;
            double sum_area = 0;

            auto current_edge =  vertices[i]->inc_edge;
            auto start_edge = vertices[i]->inc_edge;

            while(true)
            {
                // if(current_edge == nullptr)
                // {
                //     std::cout<<YELLOW<<"[WARNING]::[MeanCurvature]::Not a model which can be embedded into a halfedge, maybe there are some  non-connected vertices."<<RESET<<std::endl;
                //     break;
                // }
                const Point3 &v_next = current_edge->next_edge->des_vertex->coor;
                const Point3 &v_current = current_edge->des_vertex->coor; 
                const Point3 &v_ = current_edge->ori_vertex->coor; 
                int  angle1 = geometry::AngleType(v_current - v_, v_next - v_), angle2 = geometry::AngleType(v_ - v_current, v_next - v_current), 
                    angle3 = geometry::AngleType(v_ - v_next, v_current - v_next);

                if(angle1 > 0)
                    sum_area += 0.5 * geometry::ComputeTriangleArea(v_, v_current, v_next);
                else if(angle2 == 0 && angle3 == 0) //voronoi area
                sum_area += 0.125 * ((v_next - v_).squaredNorm() * geometry::ClampCot(v_ - v_current, v_next - v_current)
                    + (v_current - v_).squaredNorm() * geometry::ClampCot(v_ - v_next, v_current - v_next));
                else  sum_area += 0.25 * geometry::ComputeTriangleArea(v_, v_current, v_next);

                sum_angle += AngleOfVector(v_current - v_, v_next - v_);
                current_edge = current_edge->twin_edge->next_edge;
                if(current_edge == start_edge) break;
            }
            gauss_curvatures.push_back( (2 * M_PI -  sum_angle) / sum_area);
        }
    }
}
}
}