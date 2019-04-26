#ifndef IGRAPHHOME_P
#define IGRAPHHOME_P

#include "igraphhome.h"
#include "igraph_p.h"
#include "iuiwidget_global.h"

class IDataBinding ;
class IHomeConfigItem;
class HomeConfigManager;

class IUIWIDGETSHARED_EXPORT IGraphHomePrivate:public IGraphPrivate
{
  Q_DECLARE_PUBLIC(IGraphHome)
public:
  IGraphHomePrivate():
      m_curHomeConfigItem(NULL),
      m_homeConfigManage(NULL),
      m_iDataBinding(NULL){}
  virtual ~IGraphHomePrivate(){}

protected:
  IHomeConfigItem *m_curHomeConfigItem;
  HomeConfigManager *m_homeConfigManage;
  IDataBinding *m_iDataBinding;
};

#endif // IGRAPHHOME_P

