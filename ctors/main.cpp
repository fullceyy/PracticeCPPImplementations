#include <cstddef>
#include <iostream>
#include <cstring>
#include <vector>

class Mesh {
public:
    Mesh() { std::cout << "Mesh constructor called!\n"; }

    Mesh(const char* data) {
        m_Size = strlen(data);
        m_MeshData = new char[m_Size + 1];
        memcpy(m_MeshData, data, m_Size + 1);
        std::cout << "Data Mesh constructor called!\n";
    }

    Mesh(const Mesh& data) : m_Size(data.m_Size) {
        m_MeshData = new char[m_Size + 1];
        memcpy(m_MeshData, data.m_MeshData, m_Size + 1);
        std::cout << "Mesh Copy constructor called!\n";
    }

    Mesh& operator=(const Mesh& other) {
        if(m_Size != 0) {
            m_MeshData = nullptr;
        }
        m_Size = other.m_Size;
        m_MeshData = other.m_MeshData;
        return *this;
    }

    Mesh(Mesh&& data) noexcept {
        std::cout << "Mesh Move constructor called!\n";
        m_Size = data.m_Size;
        m_MeshData = data.m_MeshData;
        // m_MeshData = new char[m_Size + 1];
        data.m_MeshData = nullptr;
        data.m_Size = 0;
    }

    Mesh& operator=(Mesh&& other) {
        if(this != &other) {
            delete[] m_MeshData;
            m_MeshData = other.m_MeshData;
            m_Size = other.m_Size;

            other.m_MeshData = nullptr;
            other.m_Size = 0;
        }
        return *this;
    }

    ~Mesh() { delete[] m_MeshData; std::cout << "Mesh Destructor called!\n"; }

    friend void PrintName(const Mesh& q);

private:
    char* m_MeshData;
    size_t m_Size;
};


class Entity {
public:
    Entity() {
        std::cout << "Entity Created!\n";
    }

    ~Entity() = default;

    Entity(const Mesh& idk) : m_EntityMesh(idk) {
        // std::cout << "Entity Copy Mesh Constructor called!\n";
    }

    Entity(const char* data) : m_EntityMesh(data) {

    }

    Entity(const Entity& other) : m_EntityMesh(other.m_EntityMesh) {}
    // Entity(Mesh&& idk) : m_EntityMesh((Mesh&&)idk) {
    //     std::cout << "Entity Move constructor called!\n";
    // }

    Entity(Mesh&& idk) : m_EntityMesh(std::move(idk)) {
        std::cout << "Entity Move constructor called!\n";
    }

    Entity(Entity&& other) noexcept :
        m_EntityMesh(std::move(other.m_EntityMesh)) {

        }

    Entity& operator=(Entity&& idk) noexcept {
        if(this != &idk) {
            m_EntityMesh = std::move(idk.m_EntityMesh);
        }
        return *this;
    }

private:
    Mesh m_EntityMesh;
};

void PrintName(const Mesh& q) { std::cout << q.m_MeshData << std::endl; }

/*
 * Copy constructor => <type>(const <type>&)
 * <type>b;
 * <type>a(b);
 *
 * Copy assignment => <type>& operator=(const <type>&)
 * <type>b;
 * <type>a;
 * b = a;
 *
 * Move constructor => <type>(<type>&& other)
 * <type>A;
 * <type>B(std::move(A));
 *
 * Move assignment => <type>& operator=(<type>&&)
 * <type>A;
 * <type>B;
 * A = std::move(B);
 */

 static std::vector<int> makeVector() {
     std::vector<int> v(1000, 42);
     return v;
 }

int main() {
    // Entity e2("hey");
    // Mesh m1("Hey");
    // PrintName(m1);
    // Mesh m2("qwerty");
    // PrintName(m2);
    // m1 = m2;
    // PrintName(m1);
    Entity e1 = "hey";
    Entity e3 = e1;
    // Goal is to make
    // Entity e1("Hello");
    // Goal satisfied

    std::vector<int> vec;
    vec = makeVector();

    std::cin.get();
    return 0;
}
