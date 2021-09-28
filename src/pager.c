#include <libsqlighter.h>

pager_t *pager_open(const char *filename) {
  int fd;
  assert(0 < (fd = open(filename, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR)));

  pager_t *pager;
  assert(pager = calloc(1, sizeof(*pager)));

  pager->fd          = fd;
  pager->file_lenght = lseek(fd, 0, SEEK_END);

  return pager;
}

void *pager_get_page(pager_t *pager, uint32_t page_num) {
  if (TABLE_MAX_PAGES < page_num) {
    errx(EXIT_FAILURE,
         "Tried to fetch page number out of bounds. %d > %d\n",
         page_num,
         TABLE_MAX_PAGES);
  }

  if (!pager->pages[page_num]) {
    void *page;
    assert(page = calloc(1, PAGE_SIZE));

    const uint32_t num_pages =
        pager->file_lenght / PAGE_SIZE + !!(pager->file_lenght % PAGE_SIZE);

    if (page_num <= num_pages) {
      lseek(pager->fd, page_num * PAGE_SIZE, SEEK_SET);
      ssize_t readb = read(pager->fd, page, PAGE_SIZE);

      if (0 > readb) {
        err(EXIT_FAILURE, "Error reading file: %d", errno);
      }
    }

    pager->pages[page_num] = page;
  }

  return pager->pages[page_num];
}

void pager_flush(pager_t *pager, uint32_t page_num, uint32_t size) {
  if (!pager->pages[page_num]) {
    err(EXIT_FAILURE, "Tried to flush null page");
  }

  off_t offset = lseek(pager->fd, page_num * PAGE_SIZE, SEEK_SET);
  if (0 > offset) {
    err(EXIT_FAILURE, "Failed to seek required page to flus");
  }

  ssize_t written = write(pager->fd, pager->pages[page_num], size);
  if (0 > written) {
    err(EXIT_FAILURE, "Error writting to db-file.");
  }
}