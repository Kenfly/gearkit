#ifndef __KIT_SINGLETON_H__
#define __KIT_SINGLETON_H__


/**	单件模式范型框架。
  * 可以对任何已有类型构建一个唯一的全局对象，需要手动销毁。
  * 必须实现init函数 
  */

namespace kit {

template< typename T >
class Singleton
{
public:
    // 全局对象指针
    inline static T* instance(void)
    {
        if( !instance_ )
        {
            instance_ = new T();
            instance_->baseInit();
        }

        return instance_;
    }

    // 手动销毁实例。
    static void destroyInstance(void)
    {
        if (instance_)
        {
            delete instance_;
            instance_ = 0;
        }
    }

    // 判断实例是否已经存在
    static bool exists(void)
    {
        return !instance_;
    }

protected:
    static T* instance_;
};

template<typename T> T* Singleton<T>::instance_ = 0;

} // namespace kit

#endif // __SINGLETON_H__
