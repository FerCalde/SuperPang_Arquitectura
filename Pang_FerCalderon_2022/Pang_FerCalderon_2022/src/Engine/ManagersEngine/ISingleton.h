#pragma once

template < typename T >
class ISingleton
{
  static T *m_instance;

public:
  inline static bool HasInstance()
  {
    return m_instance != nullptr;
  }

  inline static T &GetInstance() 
  {
    CreateSingleton();
    return *m_instance; 
  }

  static void DestroySingleton() 
  { 
    if (m_instance) 
    {
      delete m_instance;
      m_instance = nullptr; 
    }
  }

  static T &CreateSingleton()
  {
    if ( !m_instance )
    {
      m_instance = new T();
    }
    return *m_instance; 
  }
};

//Singleton 
template < typename T >
T *ISingleton<T>::m_instance = nullptr;

