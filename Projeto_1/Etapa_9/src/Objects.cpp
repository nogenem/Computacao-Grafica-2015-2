#include "Objects.hpp"

std::ostream& operator<<(std::ostream& os, const Coordinate& c){
    os << "x: " << c.x << " - y: " << c.y << " - z: " << c.z << std::endl;
    return os;
}

Coordinate& Coordinate::operator+=(double step){
    this->x += step;
    this->y += step;
    this->z += step;
    return *this;
}

Coordinate& Coordinate::operator-=(double step){
    this->x -= step;
    this->y -= step;
    this->z -= step;
    return *this;
}

Coordinate& Coordinate::operator+=(const Coordinate& c){
    this->x += c.x;
    this->y += c.y;
    this->z += c.z;
    return *this;
}

Coordinate& Coordinate::operator-=(const Coordinate& c){
    this->x -= c.x;
    this->y -= c.y;
    this->z -= c.z;
    return *this;
}

Coordinate Coordinate::operator-() const{
    return Coordinate(-this->x, -this->y, -this->z);
}

Coordinate operator-(const Coordinate& c1, const Coordinate& c2){
    return Coordinate(c1.x - c2.x, c1.y - c2.y, c1.z - c2.z);
}

Coordinate& Coordinate::operator*=(const Transformation& t){
    const auto &m = t.getM();
    double x = this->x, y = this->y, z = this->z, w = this->w;
    this->x = x*m[0][0] + y*m[1][0] + z*m[2][0] + w*m[3][0];
    this->y = x*m[0][1] + y*m[1][1] + z*m[2][1] + w*m[3][1];
    this->z = x*m[0][2] + y*m[1][2] + z*m[2][2] + w*m[3][2];
    this->w = x*m[0][3] + y*m[1][3] + z*m[2][3] + w*m[3][3];
    return *this;
}

Coordinate Object::center() const{
    Coordinate c;
    int n = m_coords.size();

    for(auto &p : m_coords){
        c.x += p.x;
        c.y += p.y;
        c.z += p.z;
    }

    c.x /= n;
    c.y /= n;
    c.z /= n;
    return c;
}

Coordinate Object::nCenter() const{
    Coordinate c;
    int n = m_nCoords.size();

    for(auto &p : m_nCoords){
        c.x += p.x;
        c.y += p.y;
        c.z += p.z;
    }

    c.x /= n;
    c.y /= n;
    c.z /= n;
    return c;
}

Coordinate Object3D::center() const{
    Coordinate c;
    int n = 0;

    for(auto face : m_faceList){
        for(auto p : face.getCoords()){
            c.x += p.x;
            c.y += p.y;
            c.z += p.z;
        }
        n += face.getCoords().size();
    }

    c.x /= n;
    c.y /= n;
    c.z /= n;
    return c;
}

Coordinate Object3D::nCenter() const{
    Coordinate c;
    int n = 0;

    for(auto face : m_faceList){
        for(auto p : face.getNCoords()){
            c.x += p.x;
            c.y += p.y;
            c.z += p.z;
        }
        n += face.getNCoords().size();
    }

    c.x /= n;
    c.y /= n;
    c.z /= n;
    return c;
}

Coordinate Surface::center() const{
    Coordinate c;
    int n = 0;

    for(auto curve : m_curveList){
        for(auto p : curve.getCoords()){
            c.x += p.x;
            c.y += p.y;
            c.z += p.z;
        }
        n += curve.getCoords().size();
    }

    c.x /= n;
    c.y /= n;
    c.z /= n;
    return c;
}

Coordinate Surface::nCenter() const{
    Coordinate c;
    int n = 0;

    for(auto curve : m_curveList){
        for(auto p : curve.getNCoords()){
            c.x += p.x;
            c.y += p.y;
            c.z += p.z;
        }
        n += curve.getNCoords().size();
    }

    c.x /= n;
    c.y /= n;
    c.z /= n;
    return c;
}

void Object::transform(const Transformation& t){
    for(auto &p : m_coords)
        p *= (t);
}

void Object::transformNormalized(const Transformation& t){
    if(m_nCoords.size() > 0)
        m_nCoords.clear();
    for(auto p : m_coords)
        m_nCoords.push_back( (p *= t) );
}

void Object3D::transform(const Transformation& t){
    for(auto &face : m_faceList)
        face.transform(t);
}

void Object3D::transformNormalized(const Transformation& t){
    for(auto &face : m_faceList)
        face.transformNormalized(t);
}

void Surface::transform(const Transformation& t){
    for(auto &curve : m_curveList)
        curve.transform(t);
}

void Surface::transformNormalized(const Transformation& t){
    for(auto &curve : m_curveList)
        curve.transformNormalized(t);
}

void Object::setNCoord(const Coordinates& c){
    m_nCoords.clear();
    m_nCoords.insert(m_nCoords.end(), c.begin(), c.end());
}

void BezierCurve::generateCurve(const Coordinates& cpCoords){
    if(m_controlPoints.size() != 0)
        return;

    setControlPoints(cpCoords);
    const auto& coords = m_controlPoints;

    int numCurves = ((coords.size()-4)/3) + 1;

    for(int i = 0; i<numCurves; i++){

        // Bleding Function
        for(float t = 0; t<1; t += m_step){
            double t2 = t * t;
            double t3 = t2 * t;

            double x,y,z;
            x = (-t3 +3*t2 -3*t +1)*coords[i*3+0].x + (3*t3 -6*t2 +3*t)*coords[i*3+1].x +
                (-3*t3 +3*t2)*coords[i*3+2].x + (t3)*coords[i*3+3].x;
            y = (-t3 +3*t2 -3*t +1)*coords[i*3+0].y + (3*t3 -6*t2 +3*t)*coords[i*3+1].y +
                (-3*t3 +3*t2)*coords[i*3+2].y + (t3)*coords[i*3+3].y;
            z = (-t3 +3*t2 -3*t +1)*coords[i*3+0].z + (3*t3 -6*t2 +3*t)*coords[i*3+1].z +
                (-3*t3 +3*t2)*coords[i*3+2].z + (t3)*coords[i*3+3].z;

            m_coords.emplace_back(x,y,z);
        }
    }
}

void BSplineCurve::generateCurve(const Coordinates& cpCoords){
    if(m_controlPoints.size() != 0)
        return;

    setControlPoints(cpCoords);
    const auto& coords = m_controlPoints;

    int numCurves = coords.size()-3;

    double t = m_step;
    double t2 = t * m_step;
    double t3 = t2 * m_step;

    double n16 = 1.0/6.0;
    double n23 = 2.0/3.0;

    for(int i = 0; i < numCurves; i++){
        auto c1 = coords[i];
        auto c2 = coords[i+1];
        auto c3 = coords[i+2];
        auto c4 = coords[i+3];

        double ax = -n16 * c1.x  +0.5 * c2.x -0.5 * c3.x +n16 * c4.x;
        double bx =  0.5 * c1.x  -      c2.x +0.5 * c3.x;
        double cx = -0.5 * c1.x              +0.5 * c3.x;
        double dx =  n16 * c1.x  +n23 * c2.x +n16 * c3.x;

        double deltaX = ax*t3 +bx*t2 +cx*t;
        double deltaX3 = ax*(6*t3);
        double deltaX2 = deltaX3 +bx*(2*t2);

        double ay = -n16 * c1.y  +0.5 * c2.y -0.5 * c3.y +n16 * c4.y;
        double by =  0.5 * c1.y  -      c2.y +0.5 * c3.y;
        double cy = -0.5 * c1.y              +0.5 * c3.y;
        double dy =  n16 * c1.y  +n23 * c2.y +n16 * c3.y;

        double deltaY = ay*t3 +by*t2 +cy*t;
        double deltaY3 = ay*(6*t3);
        double deltaY2 = deltaY3 +by*(2*t2);

        double az = -n16 * c1.z  +0.5 * c2.z -0.5 * c3.z +n16 * c4.z;
        double bz =  0.5 * c1.z  -      c2.z +0.5 * c3.z;
        double cz = -0.5 * c1.z              +0.5 * c3.z;
        double dz =  n16 * c1.z  +n23 * c2.z +n16 * c3.z;

        double deltaZ = az*t3 +bz*t2 +cz*t;
        double deltaZ3 = az*(6*t3);
        double deltaZ2 = deltaZ3 +bz*(2*t2);

        double vx = dx, vy = dy, vz = dz;
        addCoordinate(vx, vy, dz);
        for(float t = 0.0; t < 1.0; t += m_step){
            double x = vx, y = vy, z = vz;

            x += deltaX;
            deltaX += deltaX2;
            deltaX2 += deltaX3;

            y += deltaY;
            deltaY += deltaY2;
            deltaY2 += deltaY3;

            z += deltaZ;
            deltaZ += deltaZ2;
            deltaZ2 += deltaZ3;

            addCoordinate(x,y,z);
            vx = x;
            vy = y;
            vz = z;
        }
    }
}

void BezierSurface::generateSurface(const Coordinates& cpCoords){
    if(m_controlPoints.size() != 0)
        return;

    setControlPoints(cpCoords);
    const auto& coords = m_controlPoints;

    int tmp3xMaxLines = 3*m_maxLines,
        tmp3xMaxCols = 3*m_maxCols;
    for(int nLine = 0; (m_maxLines != 4 && nLine <= tmp3xMaxLines) ||
            (nLine < tmp3xMaxLines); nLine += tmp3xMaxCols){
        for(int nCol = 0; nCol < m_maxCols-1; nCol += 3){

            for(float s = 0.0; s <= 1.0; s += m_step){
                double s2 = s * s;
                double s3 = s2 * s;

                BezierCurve curve("curve"+std::to_string(s), getColor());
                for(float t = 0.0; t <= 1.0; t += m_step){
                    double t2 = t * t;
                    double t3 = t2 * t;

                    curve.addCoordinate(blendingFunction(s,s2,s3,t,t2,t3,nLine,nCol,coords));
                }
                m_curveList.insert(m_curveList.end(), curve);
            }

            for(float t = 0.0; t <= 1.0; t += m_step){
                double t2 = t * t;
                double t3 = t2 * t;

                BezierCurve curve("curve"+std::to_string(t), getColor());
                for(float s = 0.0; s <= 1.0; s += m_step){
                    double s2 = s * s;
                    double s3 = s2 * s;

                    curve.addCoordinate(blendingFunction(s,s2,s3,t,t2,t3,nLine,nCol,coords));
                }
                m_curveList.insert(m_curveList.end(), curve);
            }
        }
    }
}

Coordinate BezierSurface::blendingFunction(float s, double s2, double s3,
                                    float t, double t2, double t3, int nLine, int nCol,
                                    const Coordinates& coords){
    double x = (-((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol].x +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol].x   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol].x   +(s3)*coords[m_maxCols*3+nLine+nCol].x) +3*((-s3 +3*s2 -3*s +1)*coords[1+nLine+nCol].x +(3*s3 -6*s2 +3*s)*coords[m_maxCols+1+nLine+nCol].x +(-3*s3 +3*s2)*coords[m_maxCols*2+1+nLine+nCol].x +(s3)*coords[m_maxCols*3+1+nLine+nCol].x) -3*((-s3 +3*s2 -3*s +1)*coords[2+nLine+nCol].x +(3*s3 -6*s2 +3*s)*coords[m_maxCols+2+nLine+nCol].x +(-3*s3 +3*s2)*coords[m_maxCols*2+2+nLine+nCol].x +(s3)*coords[m_maxCols*3+2+nLine+nCol].x) +((-s3 +3*s2 -3*s +1)*coords[3+nLine+nCol].x +(3*s3 -6*s2 +3*s)*coords[m_maxCols+3+nLine+nCol].x +(-3*s3 +3*s2)*coords[m_maxCols*2+3+nLine+nCol].x +(s3)*coords[m_maxCols*3+3+nLine+nCol].x))*t3
     +(3*((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol].x +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol].x   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol].x   +(s3)*coords[m_maxCols*3+nLine+nCol].x) -6*((-s3 +3*s2 -3*s +1)*coords[1+nLine+nCol].x +(3*s3 -6*s2 +3*s)*coords[m_maxCols+1+nLine+nCol].x +(-3*s3 +3*s2)*coords[m_maxCols*2+1+nLine+nCol].x +(s3)*coords[m_maxCols*3+1+nLine+nCol].x) +3*((-s3 +3*s2 -3*s +1)*coords[2+nLine+nCol].x +(3*s3 -6*s2 +3*s)*coords[m_maxCols+2+nLine+nCol].x +(-3*s3 +3*s2)*coords[m_maxCols*2+2+nLine+nCol].x +(s3)*coords[m_maxCols*3+2+nLine+nCol].x))*t2
     +(-3*((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol].x +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol].x   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol].x   +(s3)*coords[m_maxCols*3+nLine+nCol].x) +3*((-s3 +3*s2 -3*s +1)*coords[1+nLine+nCol].x +(3*s3 -6*s2 +3*s)*coords[m_maxCols+1+nLine+nCol].x +(-3*s3 +3*s2)*coords[m_maxCols*2+1+nLine+nCol].x +(s3)*coords[m_maxCols*3+1+nLine+nCol].x))*t
     +(((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol].x +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol].x   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol].x   +(s3)*coords[m_maxCols*3+nLine+nCol].x));
    double y = (-((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol].y +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol].y   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol].y   +(s3)*coords[m_maxCols*3+nLine+nCol].y) +3*((-s3 +3*s2 -3*s +1)*coords[1+nLine+nCol].y +(3*s3 -6*s2 +3*s)*coords[m_maxCols+1+nLine+nCol].y +(-3*s3 +3*s2)*coords[m_maxCols*2+1+nLine+nCol].y +(s3)*coords[m_maxCols*3+1+nLine+nCol].y) -3*((-s3 +3*s2 -3*s +1)*coords[2+nLine+nCol].y +(3*s3 -6*s2 +3*s)*coords[m_maxCols+2+nLine+nCol].y +(-3*s3 +3*s2)*coords[m_maxCols*2+2+nLine+nCol].y +(s3)*coords[m_maxCols*3+2+nLine+nCol].y) +((-s3 +3*s2 -3*s +1)*coords[3+nLine+nCol].y +(3*s3 -6*s2 +3*s)*coords[m_maxCols+3+nLine+nCol].y +(-3*s3 +3*s2)*coords[m_maxCols*2+3+nLine+nCol].y +(s3)*coords[m_maxCols*3+3+nLine+nCol].y))*t3
     +(3*((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol].y +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol].y   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol].y   +(s3)*coords[m_maxCols*3+nLine+nCol].y) -6*((-s3 +3*s2 -3*s +1)*coords[1+nLine+nCol].y +(3*s3 -6*s2 +3*s)*coords[m_maxCols+1+nLine+nCol].y +(-3*s3 +3*s2)*coords[m_maxCols*2+1+nLine+nCol].y +(s3)*coords[m_maxCols*3+1+nLine+nCol].y) +3*((-s3 +3*s2 -3*s +1)*coords[2+nLine+nCol].y +(3*s3 -6*s2 +3*s)*coords[m_maxCols+2+nLine+nCol].y +(-3*s3 +3*s2)*coords[m_maxCols*2+2+nLine+nCol].y +(s3)*coords[m_maxCols*3+2+nLine+nCol].y))*t2
     +(-3*((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol].y +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol].y   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol].y   +(s3)*coords[m_maxCols*3+nLine+nCol].y) +3*((-s3 +3*s2 -3*s +1)*coords[1+nLine+nCol].y +(3*s3 -6*s2 +3*s)*coords[m_maxCols+1+nLine+nCol].y +(-3*s3 +3*s2)*coords[m_maxCols*2+1+nLine+nCol].y +(s3)*coords[m_maxCols*3+1+nLine+nCol].y))*t
     +(((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol].y +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol].y   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol].y   +(s3)*coords[m_maxCols*3+nLine+nCol].y));
    double z = (-((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol].z +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol].z   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol].z   +(s3)*coords[m_maxCols*3+nLine+nCol].z) +3*((-s3 +3*s2 -3*s +1)*coords[1+nLine+nCol].z +(3*s3 -6*s2 +3*s)*coords[m_maxCols+1+nLine+nCol].z +(-3*s3 +3*s2)*coords[m_maxCols*2+1+nLine+nCol].z +(s3)*coords[m_maxCols*3+1+nLine+nCol].z) -3*((-s3 +3*s2 -3*s +1)*coords[2+nLine+nCol].z +(3*s3 -6*s2 +3*s)*coords[m_maxCols+2+nLine+nCol].z +(-3*s3 +3*s2)*coords[m_maxCols*2+2+nLine+nCol].z +(s3)*coords[m_maxCols*3+2+nLine+nCol].z) +((-s3 +3*s2 -3*s +1)*coords[3+nLine+nCol].z +(3*s3 -6*s2 +3*s)*coords[m_maxCols+3+nLine+nCol].z +(-3*s3 +3*s2)*coords[m_maxCols*2+3+nLine+nCol].z +(s3)*coords[m_maxCols*3+3+nLine+nCol].z))*t3
     +(3*((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol].z +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol].z   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol].z   +(s3)*coords[m_maxCols*3+nLine+nCol].z) -6*((-s3 +3*s2 -3*s +1)*coords[1+nLine+nCol].z +(3*s3 -6*s2 +3*s)*coords[m_maxCols+1+nLine+nCol].z +(-3*s3 +3*s2)*coords[m_maxCols*2+1+nLine+nCol].z +(s3)*coords[m_maxCols*3+1+nLine+nCol].z) +3*((-s3 +3*s2 -3*s +1)*coords[2+nLine+nCol].z +(3*s3 -6*s2 +3*s)*coords[m_maxCols+2+nLine+nCol].z +(-3*s3 +3*s2)*coords[m_maxCols*2+2+nLine+nCol].z +(s3)*coords[m_maxCols*3+2+nLine+nCol].z))*t2
     +(-3*((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol].z +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol].z   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol].z   +(s3)*coords[m_maxCols*3+nLine+nCol].z) +3*((-s3 +3*s2 -3*s +1)*coords[1+nLine+nCol].z +(3*s3 -6*s2 +3*s)*coords[m_maxCols+1+nLine+nCol].z +(-3*s3 +3*s2)*coords[m_maxCols*2+1+nLine+nCol].z +(s3)*coords[m_maxCols*3+1+nLine+nCol].z))*t
     +(((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol].z +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol].z   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol].z   +(s3)*coords[m_maxCols*3+nLine+nCol].z));

    return Coordinate(x,y,z);
}

void BSplineSurface::generateSurface(const Coordinates& cpCoords){
    if(m_controlPoints.size() != 0)
        return;

    setControlPoints(cpCoords);
    const auto& coords = m_controlPoints;

    double n16 = 1.0/6.0;
    double n23 = 2.0/3.0;

    for(int nLine = 0; (m_maxLines != 4 && nLine <= m_maxLines) ||
                (nLine < m_maxLines); nLine += m_maxCols){
        for(int nCol = 0; nCol <= m_maxCols-4; nCol += 1){

            for(float s = 0.0; s <= 1.0; s += m_step){
                double s2 = s * s;
                double s3 = s2 * s;

                BSplineCurve curve("curve"+std::to_string(s), getColor());
                for(float t = 0.0; t <= 1.0; t += m_step){
                    double t2 = t * t;
                    double t3 = t2 * t;

                    curve.addCoordinate(blendingFunction(s,s2,s3,t,t2,t3,n16,n23,nLine,nCol,coords));
                }
                m_curveList.insert(m_curveList.end(), curve);
            }

            for(float t = 0.0; t <= 1.0; t += m_step){
                double t2 = t * t;
                double t3 = t2 * t;

                BSplineCurve curve("curve"+std::to_string(t), getColor());
                for(float s = 0.0; s <= 1.0; s += m_step){
                    double s2 = s * s;
                    double s3 = s2 * s;

                    curve.addCoordinate(blendingFunction(s,s2,s3,t,t2,t3,n16,n23,nLine,nCol,coords));
                }
                m_curveList.insert(m_curveList.end(), curve);
            }
        }
    }
}

Coordinate BSplineSurface::blendingFunction(float s, double s2, double s3,
                                    float t, double t2, double t3,
                                    double n16, double n23, int nLine, int nCol,
                                    const Coordinates& coords){
    double x = (-n16*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[0+nLine+nCol].x +(0.5*s3 -s2 +n23)*coords[m_maxCols+nLine+nCol].x   +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+nLine+nCol].x   +(n16*s3)*coords[m_maxCols*3+nLine+nCol].x) +0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[1+nLine+nCol].x +(0.5*s3 -s2 +n23)*coords[m_maxCols+1+nLine+nCol].x +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+1+nLine+nCol].x +(n16*s3)*coords[m_maxCols*3+1+nLine+nCol].x) -0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[2+nLine+nCol].x +(0.5*s3 -s2 +n23)*coords[m_maxCols+2+nLine+nCol].x +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+2+nLine+nCol].x +(n16*s3)*coords[m_maxCols*3+2+nLine+nCol].x) +n16*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[3+nLine+nCol].x +(0.5*s3 -s2 +n23)*coords[m_maxCols+3+nLine+nCol].x +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+3+nLine+nCol].x +(n16*s3)*coords[m_maxCols*3+3+nLine+nCol].x))*t3
     +(0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[0+nLine+nCol].x +(0.5*s3 -s2 +n23)*coords[m_maxCols+nLine+nCol].x   +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+nLine+nCol].x   +(n16*s3)*coords[m_maxCols*3+nLine+nCol].x) -((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[1+nLine+nCol].x +(0.5*s3 -s2 +n23)*coords[m_maxCols+1+nLine+nCol].x +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+1+nLine+nCol].x +(n16*s3)*coords[m_maxCols*3+1+nLine+nCol].x) +0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[2+nLine+nCol].x +(0.5*s3 -s2 +n23)*coords[m_maxCols+2+nLine+nCol].x +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+2+nLine+nCol].x +(n16*s3)*coords[m_maxCols*3+2+nLine+nCol].x))*t2
     +(-0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[0+nLine+nCol].x +(0.5*s3 -s2 +n23)*coords[m_maxCols+nLine+nCol].x   +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+nLine+nCol].x   +(n16*s3)*coords[m_maxCols*3+nLine+nCol].x) +0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[2+nLine+nCol].x +(0.5*s3 -s2 +n23)*coords[m_maxCols+2+nLine+nCol].x +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+2+nLine+nCol].x +(n16*s3)*coords[m_maxCols*3+2+nLine+nCol].x))*t
     +(n16*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[0+nLine+nCol].x +(0.5*s3 -s2 +n23)*coords[m_maxCols+nLine+nCol].x   +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+nLine+nCol].x   +(n16*s3)*coords[m_maxCols*3+nLine+nCol].x) +n23*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[1+nLine+nCol].x +(0.5*s3 -s2 +n23)*coords[m_maxCols+1+nLine+nCol].x +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+1+nLine+nCol].x +(n16*s3)*coords[m_maxCols*3+1+nLine+nCol].x) +n16*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[2+nLine+nCol].x +(0.5*s3 -s2 +n23)*coords[m_maxCols+2+nLine+nCol].x +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+2+nLine+nCol].x +(n16*s3)*coords[m_maxCols*3+2+nLine+nCol].x));
    double y = (-n16*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[0+nLine+nCol].y +(0.5*s3 -s2 +n23)*coords[m_maxCols+nLine+nCol].y   +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+nLine+nCol].y   +(n16*s3)*coords[m_maxCols*3+nLine+nCol].y) +0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[1+nLine+nCol].y +(0.5*s3 -s2 +n23)*coords[m_maxCols+1+nLine+nCol].y +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+1+nLine+nCol].y +(n16*s3)*coords[m_maxCols*3+1+nLine+nCol].y) -0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[2+nLine+nCol].y +(0.5*s3 -s2 +n23)*coords[m_maxCols+2+nLine+nCol].y +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+2+nLine+nCol].y +(n16*s3)*coords[m_maxCols*3+2+nLine+nCol].y) +n16*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[3+nLine+nCol].y +(0.5*s3 -s2 +n23)*coords[m_maxCols+3+nLine+nCol].y +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+3+nLine+nCol].y +(n16*s3)*coords[m_maxCols*3+3+nLine+nCol].y))*t3
     +(0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[0+nLine+nCol].y +(0.5*s3 -s2 +n23)*coords[m_maxCols+nLine+nCol].y   +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+nLine+nCol].y   +(n16*s3)*coords[m_maxCols*3+nLine+nCol].y) -((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[1+nLine+nCol].y +(0.5*s3 -s2 +n23)*coords[m_maxCols+1+nLine+nCol].y +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+1+nLine+nCol].y +(n16*s3)*coords[m_maxCols*3+1+nLine+nCol].y) +0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[2+nLine+nCol].y +(0.5*s3 -s2 +n23)*coords[m_maxCols+2+nLine+nCol].y +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+2+nLine+nCol].y +(n16*s3)*coords[m_maxCols*3+2+nLine+nCol].y))*t2
     +(-0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[0+nLine+nCol].y +(0.5*s3 -s2 +n23)*coords[m_maxCols+nLine+nCol].y   +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+nLine+nCol].y   +(n16*s3)*coords[m_maxCols*3+nLine+nCol].y) +0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[2+nLine+nCol].y +(0.5*s3 -s2 +n23)*coords[m_maxCols+2+nLine+nCol].y +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+2+nLine+nCol].y +(n16*s3)*coords[m_maxCols*3+2+nLine+nCol].y))*t
     +(n16*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[0+nLine+nCol].y +(0.5*s3 -s2 +n23)*coords[m_maxCols+nLine+nCol].y   +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+nLine+nCol].y   +(n16*s3)*coords[m_maxCols*3+nLine+nCol].y) +n23*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[1+nLine+nCol].y +(0.5*s3 -s2 +n23)*coords[m_maxCols+1+nLine+nCol].y +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+1+nLine+nCol].y +(n16*s3)*coords[m_maxCols*3+1+nLine+nCol].y) +n16*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[2+nLine+nCol].y +(0.5*s3 -s2 +n23)*coords[m_maxCols+2+nLine+nCol].y +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+2+nLine+nCol].y +(n16*s3)*coords[m_maxCols*3+2+nLine+nCol].y));
    double z = (-n16*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[0+nLine+nCol].z +(0.5*s3 -s2 +n23)*coords[m_maxCols+nLine+nCol].z   +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+nLine+nCol].z   +(n16*s3)*coords[m_maxCols*3+nLine+nCol].z) +0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[1+nLine+nCol].z +(0.5*s3 -s2 +n23)*coords[m_maxCols+1+nLine+nCol].z +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+1+nLine+nCol].z +(n16*s3)*coords[m_maxCols*3+1+nLine+nCol].z) -0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[2+nLine+nCol].z +(0.5*s3 -s2 +n23)*coords[m_maxCols+2+nLine+nCol].z +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+2+nLine+nCol].z +(n16*s3)*coords[m_maxCols*3+2+nLine+nCol].z) +n16*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[3+nLine+nCol].z +(0.5*s3 -s2 +n23)*coords[m_maxCols+3+nLine+nCol].z +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+3+nLine+nCol].z +(n16*s3)*coords[m_maxCols*3+3+nLine+nCol].z))*t3
     +(0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[0+nLine+nCol].z +(0.5*s3 -s2 +n23)*coords[m_maxCols+nLine+nCol].z   +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+nLine+nCol].z   +(n16*s3)*coords[m_maxCols*3+nLine+nCol].z) -((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[1+nLine+nCol].z +(0.5*s3 -s2 +n23)*coords[m_maxCols+1+nLine+nCol].z +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+1+nLine+nCol].z +(n16*s3)*coords[m_maxCols*3+1+nLine+nCol].z) +0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[2+nLine+nCol].z +(0.5*s3 -s2 +n23)*coords[m_maxCols+2+nLine+nCol].z +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+2+nLine+nCol].z +(n16*s3)*coords[m_maxCols*3+2+nLine+nCol].z))*t2
     +(-0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[0+nLine+nCol].z +(0.5*s3 -s2 +n23)*coords[m_maxCols+nLine+nCol].z   +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+nLine+nCol].z   +(n16*s3)*coords[m_maxCols*3+nLine+nCol].z) +0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[2+nLine+nCol].z +(0.5*s3 -s2 +n23)*coords[m_maxCols+2+nLine+nCol].z +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+2+nLine+nCol].z +(n16*s3)*coords[m_maxCols*3+2+nLine+nCol].z))*t
     +(n16*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[0+nLine+nCol].z +(0.5*s3 -s2 +n23)*coords[m_maxCols+nLine+nCol].z   +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+nLine+nCol].z   +(n16*s3)*coords[m_maxCols*3+nLine+nCol].z) +n23*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[1+nLine+nCol].z +(0.5*s3 -s2 +n23)*coords[m_maxCols+1+nLine+nCol].z +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+1+nLine+nCol].z +(n16*s3)*coords[m_maxCols*3+1+nLine+nCol].z) +n16*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[2+nLine+nCol].z +(0.5*s3 -s2 +n23)*coords[m_maxCols+2+nLine+nCol].z +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+2+nLine+nCol].z +(n16*s3)*coords[m_maxCols*3+2+nLine+nCol].z));

    return Coordinate(x,y,z);
}
