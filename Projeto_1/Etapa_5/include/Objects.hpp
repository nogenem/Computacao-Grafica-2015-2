#ifndef OBJECTS_H
#define OBJECTS_H

#include <gtk/gtk.h>
#include <iostream>
#include <string>
#include <vector>
#include "Transformation.hpp"

class Transformation;

class Coordinate
{
    public:
        Coordinate(){}
        Coordinate(double cx, double cy) :
            x(cx), y(cy), z(1){}
        virtual ~Coordinate(){};

        Coordinate& operator+=(double step);
        Coordinate& operator-=(double step);
        Coordinate& operator+=(const Coordinate& c);
        Coordinate& operator-=(const Coordinate& c);
        Coordinate& operator*=(const Transformation& t);
        bool operator==(const Coordinate& c)
            { return (this->x==c.x && this->y==c.y); }

        double x = 0, y = 0, z = 1;
};

Coordinate operator-(const Coordinate& c1, const Coordinate& c2);

typedef std::vector<Coordinate> Coordinates;
enum class ObjType { OBJECT, POINT, LINE, POLYGON };

class Object
{
    public:
        Object(const std::string& name) :
            m_name(name) {}
        Object(const std::string& name, const GdkRGBA& c) :
            m_name(name), m_color({c.red,c.green,c.blue,c.alpha}) {}
        virtual ~Object() {}

        const std::string& getName() const { return m_name; }

        const GdkRGBA& getColor() const { return m_color; }

        virtual ObjType getType() const { return ObjType::OBJECT; }
		virtual std::string getTypeName() const { return "Object"; }

        Coordinates& getCoords() {return m_coords;}
        Coordinate& getCoord(int index) { return m_coords[index]; }
        int getCoordsSize() const { return m_coords.size(); }

        Coordinates& getNCoords() {return m_nCoords;}
		Coordinate& getNCoord(int index) { return m_nCoords[index]; }
		void setNCoord(const Coordinates& c);
		int getNCoordsSize() const { return m_nCoords.size(); }

        Coordinate center() const;
        Coordinate nCenter() const;
        void transform(const Transformation& t);
        void transformNormalized(const Transformation& t);

        bool operator==(const Object& other)
            { return this->getName() == other.getName(); }
        Object& operator*(){ return *this; }

		virtual void addCoordinate(double x, double y) { m_coords.emplace_back(x,y); }
		void addCoordinate(const Coordinate& p) { m_coords.push_back(p); }

    protected:
        void addCoordinate(const Coordinates& coords)
            { m_coords.insert(m_coords.end(), coords.begin(), coords.end()); }

    protected:
        std::string m_name;
        GdkRGBA m_color{};// [inicializada como preta]
        Coordinates m_coords;
        Coordinates m_nCoords; // Coordenadas normalizadadas
};

class Point : public Object
{
    public:
        Point(const std::string& name) :
            Object(name) {}
        Point(const std::string& name, const GdkRGBA& color) :
            Object(name,color) {}
		Point(const std::string& name, const GdkRGBA& color, double x, double y) :
            Object(name,color) { addCoordinate(x,y); }
        Point(const std::string& name, const GdkRGBA& color, const Coordinate& p) :
            Object(name,color) { addCoordinate(p); }

        virtual ObjType getType() const { return ObjType::POINT; }
		virtual std::string getTypeName() const { return "Point"; }
};

class Line : public Object
{
    public:
        Line(const std::string& name) :
            Object(name) {}
        Line(const std::string& name, const GdkRGBA& color) :
            Object(name,color) {}
		Line(const std::string& name, const GdkRGBA& color, const Coordinates& coords) :
            Object(name,color) { addCoordinate(coords); }

        virtual ObjType getType() const { return ObjType::LINE; }
		virtual std::string getTypeName() const { return "Line"; }
};

class Polygon : public Object
{
    public:
        Polygon(const std::string& name) :
            Object(name) {}
        Polygon(const std::string& name, const GdkRGBA& color) :
            Object(name,color) {}
		Polygon(const std::string& name, const GdkRGBA& color, const Coordinates& coords) :
            Object(name,color) { addCoordinate(coords); }
        Polygon(const std::string& name, const GdkRGBA& color, bool filled, const Coordinates& coords) :
            Object(name,color) { m_filled = filled; addCoordinate(coords); }

        virtual ObjType getType() const { return ObjType::POLYGON; }
		virtual std::string getTypeName() const { return "Polygon"; }

        bool filled() const { return m_filled; }
        void setFilled(bool v){ m_filled = v; }

    private:
        bool m_filled = false;
};
#endif // OBJECTS_H
