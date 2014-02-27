#include <sstream>

#include ".\revrecord.h"
#include "core.h"

//////////////////////////////////////////////////////////////////////////

String RevArg::string_mean() const
{
  std::stringstream rv;
  rv << core().data().PTs[mean];
  rv << " ";
  rv << core().data().MLs[member];
  rv << " системы ";
  rv << core().link_label(end);
  rv << "  в системе ";
  rv << core().link_label(start);
  rv << " равно ";
  rv << value;
  rv << ". Штраф ";
  rv << penalty;
  rv << ".";
  return rv.str();
}

void RevArg::apply_to_data(OptData& data) const
{
  static MMatrix<MSymb> matr(4, 4);
  
  switch (mean)
  {
  case 0:
    core().drGetMatrixT(start, end, matr);
    break;
  case 1:
    core().drGetMatrixV(start, end, matr);
    break;
  case 2:
    core().drGetMatrixC(start, end, matr);
    break;
  };
  
  data.func += ((matr[member] - value).absoluted()) * penalty;
}

//////////////////////////////////////////////////////////////////////////

RevRecord::RevRecord(void)
{
}

RevRecord::~RevRecord(void)
{
}

int RevRecord::add_arg(const RevArg& arg)
{
  m_args.push_back(arg);
  return int(m_args.size()) - 1;
}

RevArg& RevRecord::arg(int index)
{
  return m_args[index];
}

int RevRecord::arg_count()const
{
  return int(m_args.size());
}

void RevRecord::del_arg(int index)
{
  RevArgs::iterator it = m_args.begin();
  std::advance(it, index);
  m_args.erase(it);
}

bool RevRecord::consists(const char* data) const
{
  return data == "rev_record";
}

void RevRecord::clear()
{
  m_args.clear();
}

void RevRecord::get_data()
{
}

void RevRecord::apply_data() const
{
}