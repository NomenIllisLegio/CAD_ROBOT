#pragma once

class serializers
{
public:
  serializers(void);
  ~serializers(void);

  static void store(CArchive& a, Core& core);
  static void load(CArchive& a, Core& core);

  static void store(CArchive& a, Link& link);
  static void load(CArchive& a, Link& link);

  static void store(CArchive& a, Record& rec);
  static void load(CArchive& a, Record& rec);

  static void store(CArchive& a, RevRecord& rev_r);
  static void load(CArchive& a, RevRecord& rev_r);

  static void store(CArchive& a, TrajRecord& rev_r);
  static void load(CArchive& a, TrajRecord& rev_r);

  static void store(CArchive& a, PosRecord& rev_r);
  static void load(CArchive& a, PosRecord& rev_r);
};
