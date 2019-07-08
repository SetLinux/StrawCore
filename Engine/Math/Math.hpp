#pragma once
#define _USE_MATH_DEFINES
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_SIMD_AVX2

#include <math.h>
#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <type_traits>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>

struct XVector {

  float x, y, z , w;
  XVector() : x(0), y(0), z(0), w(0) {}
  template<class Archive>  
  void serialize(Archive & ar, const unsigned int /* file_version */){
    ar & BOOST_SERIALIZATION_NVP(x) & BOOST_SERIALIZATION_NVP(y) & BOOST_SERIALIZATION_NVP(z) & BOOST_SERIALIZATION_NVP(w);
    }
  XVector(float x, float y) {
    this->x = x;
    this->y = y;
    this->z = 0;
    this->w = 0;
  }
  XVector(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = 0;
  }

  XVector(float x, float y, float z,float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
  }
  XVector Normalize() {
    float length = std::sqrt(std::pow(this->x,2) + std::pow(this->y,2));
    if(length == 0) return *this;
    return *this / length; 
  }
  // Vector On Vector Operators
  XVector operator+(const XVector &other) const {
    return XVector(x + other.x, y + other.y, z + other.z);
  }
  XVector operator-(const XVector &other) const {
    return XVector(x - other.x, y - other.y, z - other.z);
  }

  XVector operator*(const XVector &other) const {
    return XVector(x * other.x, y * other.y, z * other.z);
  }

  XVector operator/(const XVector &other) const {
    return XVector(x / other.x, y / other.y, z / other.z);
  }

  // Float Operators OverLoading
  XVector operator+(const float &other) const {
    return XVector(x + other, y + other, z + other);
  }
  XVector operator-(const float &other) const {
    return XVector(x - other, y - other, z - other);
  }

  XVector operator*(const float &other) const {
    return XVector(x * other, y * other, z * other);
  }

  XVector operator/(const float &other) const {
    return XVector(x / other, y / other, z / other);
  }
   // Logical Operators
  bool operator==(const XVector &other) const {
    return (x == other.x && y == other.y && z == other.z);
  }
  bool operator!=(const XVector &other) const {
    return (x != other.x || y != other.y || z != other.z);
  }
  XVector operator+=(const XVector &other) const{
    return XVector(this->x + other.x , this->y + other.y);
  }

  XVector operator*(const glm::mat4& other) {

	  glm::vec4 temp = glm::vec4(x, y, z, 1.0f);
	  glm::vec4 endtemp = temp * other;
	  return XVector(endtemp.x, endtemp.y);
  }
    
  template <typename T> static XVector fromVec(const T& other, bool zExist);
  template <class T> static XVector fromVec(const T& other);
  template <typename T> static T ToVec(const XVector& other, bool zExist);
  template <typename T> static T ToVec(const XVector& other);

  static XVector Interpolate(const XVector& previous, const XVector& current,
                              float alpha) {
    // state = currentState * alpha + previousState * (1-alpha)
    XVector res(0, 0, 0);
    float oneMinusRatio = 1 - alpha;
    res = (current * alpha) + (previous * (oneMinusRatio));
    return res;
  }
  static float Distance(const XVector& pointa, const XVector& pointb) {
    float undersqrt =
      std::pow(pointb.x - pointa.x, 2) + std::pow(pointb.y - pointa.y, 2);
    float result = std::sqrt(undersqrt);
    return result;
  }
  static float Dot(const XVector& pointa,const XVector& pointb){
    return (pointa.x * pointb.x ) + (pointa.y * pointb.y);
  }
};
// Just Some Template Work
template <class T> T XVector::ToVec(const XVector& other, bool zExist) {
  T x;
  x.x = other.x;
  x.y = other.y;
  if (zExist)
    x.z = other.z;
  return x;
}
static float Radians(float degrees){
  float multiplier= M_PI / 180.f;
  return degrees * multiplier;
}
//Fuck Math
//Never Judge me if this has some serious wrong math i copied it from the StackOverFlow post or something the syntax was a bit different :(
static XVector Rotate(XVector point, float angle, XVector center_of_rotation)
{
	float sinX = sin(angle);
	float cosX = cos(angle);
	XVector temppoint = XVector(point.x - center_of_rotation.x, point.y - center_of_rotation.y);
	XVector RotatedPoint = XVector(temppoint.x * cosX - (temppoint.y * sinX), temppoint.x * sinX + temppoint.y * cosX);

	return RotatedPoint+center_of_rotation;
}
//Mostly used for GUI Elements (LighterWeight than using Box2D World for GUI)
static bool isPointInsideRect(XVector point,XVector rectPos,XVector rectScale,float rectrotation = 0) {
	XVector rpoint = Rotate(point, Radians(rectrotation), rectPos);
	return (rpoint.x > rectPos.x - (rectScale.x + 1) / 2 && rpoint.x < rectPos.x + (rectScale.x + 1) / 2 &&
			rpoint.y > rectPos.y - (rectScale.y + 1) / 2 && rpoint.y < rectPos.y + (rectScale.y + 1) / 2);
}

inline std::ostream &operator<<(std::ostream &os, const XVector &vec) {
  os << "X : " << vec.x << " Y : " << vec.y << " Z : " << vec.z;
  return os;
}

template <typename T> inline XVector XVector::fromVec(const T& other, bool zExist) {
  return XVector(other.x, other.y, zExist ? other.z : 0);
}
// Just Some Template Work
template <class T> T XVector::ToVec(const XVector& other) {
  T x;
  x.x = other.x;
  x.y = other.y;
  return x;
}

template <typename T> inline XVector XVector::fromVec(const T& other) {
  return XVector(other.x, other.y, 0);
}

struct Vertex {
  XVector position;
  XVector TexCoord;
};
