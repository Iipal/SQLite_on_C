#ifndef __LIBSQLIGHTER_ROW_H__
#define __LIBSQLIGHTER_ROW_H__

#define COLUMN_USERNAME_SIZE 31
#define COLUMN_EMAIL_SIZE    255

typedef struct {
  uint32_t id;
  char     username[COLUMN_USERNAME_SIZE + 1];
  char     email[COLUMN_EMAIL_SIZE + 1];
} row_t;

#define size_of_attribute(Struct, Attribute) sizeof(((Struct *)0)->Attribute)

#define ID_SIZE       (size_of_attribute(row_t, id))
#define USERNAME_SIZE (size_of_attribute(row_t, username))
#define EMAIL_SIZE    (size_of_attribute(row_t, email))
#define ROW_SIZE      (ID_SIZE + USERNAME_SIZE + EMAIL_SIZE)

#define ID_OFFSET       (offsetof(row_t, id))
#define USERNAME_OFFSET (offsetof(row_t, username))
#define EMAIL_OFFSET    (offsetof(row_t, email))

void serialize_row(row_t *src, void *dst);
void deserialize_row(void *src, row_t *dst);

static inline void print_row(row_t *restrict row) {
  printf("(%d, %s | %p, %s | %p)\n",
         row->id,
         row->username,
         row->username,
         row->email,
         row->email);
};

#endif /* __LIBSQLIGHTER_ROW_H__ */
