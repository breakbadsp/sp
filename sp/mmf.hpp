#ifndef MMF_HPP
#define MMF_HPP

#include <cstring>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cassert>

#include <string>

const int PAGE_SIZE = sysconf(_SC_PAGESIZE);

namespace sp {
  class mmf {
public:
    mmf() = default;

    mmf(const std::string &p_name, size_t p_size, size_t p_offset = 0, int p_flags = 0) {

      if (p_flags & O_RDWR)
        OpenFileForReadWrite(p_name, p_size);
      else
        OpenFileForReadOnly(p_name, p_size);

      offset_ = GetPageAlignedSize(p_offset);
      mapped_size_ = p_size;
      name_ = p_name;
      addr_ = (char*) mmap(nullptr, mapped_size_, p_flags, MAP_SHARED, fd_, offset_);
      if (addr_ == MAP_FAILED) {
        perror("Failed to map file");
        close(fd_);
        exit(EXIT_FAILURE);
      }
      start_addr_ = static_cast<char *>(addr_);
    }

    ~mmf() {
      if (start_addr_ != nullptr && start_addr_ != MAP_FAILED) {
        if (munmap(start_addr_, mapped_size_) < 0) {
          perror("Failed to unmap file");
        }
      }
      if (fd_ > 0) {
        close(fd_);
      }
    }

    bool end(char* ptr) const {
      return ptr >= static_cast<char *>(start_addr_) + mapped_size_;
    }


    std::string_view ReadFrom(std::size_t p_start, std::size_t p_size) const {
      std::string_view view(static_cast<char *>(start_addr_) + p_start, p_size);
      return view;
    }

    std::string_view ReadAllFrom(std::size_t p_start) {
      std::string_view view(static_cast<char *>(start_addr_) + p_start, mapped_size_ - p_start);
      return view;
    }

    std::string_view ReadAllFrom(std::size_t p_start, char p_delim) {
      assert(p_start < mapped_size_ && "Start position is out of bounds");
      char *start = static_cast<char *>(start_addr_) + p_start;
      assert(start < static_cast<char *>(start_addr_) + mapped_size_ && "Start position is out of bounds");
      assert(start_addr_ != nullptr && "Start address is null");
      assert(start != nullptr && "Start pointer with offset is null");
      if (end(start)) {
        return std::string_view();
      }

      size_t len = 0;
      auto* end_ptr = start;
      while (end_ptr && !end(end_ptr) && *end_ptr != p_delim && *end_ptr != '\0') {
        ++end_ptr;
      }
      len = end_ptr - start;
      std::string_view view(start, len);
      return view;
    }

    std::string_view Read(size_t p_size) const {
      // TODO:: Add some validations
      const std::string_view view(static_cast<char *>(addr_) + offset_, p_size);
      return view;
    }

    std::string_view Read(size_t p_size) {
      // TODO:: Add some validations
      const std::string_view view(static_cast<char *>(addr_) + offset_, p_size);
      addr_ += p_size;
      return view;
    }

    std::string_view Read(char p_delim) const {
      // TODO:: Add validations for EOF and p_delim
      const auto *start = static_cast<char *>(addr_);
      size_t len = 0;
      auto *end = start;
      while (*end != p_delim && *end != '\0' && end < static_cast<char *>(addr_) + mapped_size_) {
        ++end;
      }
      len = end - start;
      std::string_view view(start, len);
      return view;
    }

    std::string_view Read(char p_delim) {
      // TODO:: Add validations for EOF and p_delim
      const auto *start = static_cast<char *>(addr_);
      size_t len = 0;
      auto *end = start;
      while (*end != p_delim && *end != '\0' && end < static_cast<char *>(addr_) + mapped_size_) {
        ++end;
      }
      len = end - start;
      std::string_view view(start, len);
      addr_ += len + 1; // Move past the delimiter
      return view;
    }

    void Append(const std::string_view &p_data) {
      // TODO:: Add some validations
      std::memcpy(static_cast<char *>(addr_), p_data.data(), p_data.size());
      addr_ += p_data.size();
    }

    void Append(const std::string_view &p_data, char p_delim) {
      std::memcpy(static_cast<char *>(addr_), p_data.data(), p_data.size());
      addr_ += p_data.size();
      *static_cast<char *>(addr_) = p_delim; // Add delimiter
    }

private:
    int fd_{0};
    char* addr_{nullptr};
    char* start_addr_{nullptr};
    size_t mapped_size_{0};
    std::string name_{};
    size_t offset_{0};

    // utility functions
    void OpenFileForReadWrite(const std::string &p_name, size_t p_size) {
      fd_ = open(p_name.c_str(), O_RDWR | O_CREAT, 0666);
      if (fd_ < 0) {
        perror("Failed to open file for read/write");
        exit(EXIT_FAILURE);
      }
      if (ftruncate(fd_, p_size) < 0) {
        perror("Failed to set file size");
        close(fd_);
        exit(EXIT_FAILURE);
      }
    }

    void OpenFileForReadOnly(const std::string &p_name, size_t p_size) {
      fd_ = open(p_name.c_str(), O_RDONLY);
      if (fd_ < 0) {
        perror("Failed to open file for read only");
        exit(EXIT_FAILURE);
      }
      struct stat sb;
      if (fstat(fd_, &sb) < 0) {
        perror("Failed to get file size");
        close(fd_);
        exit(EXIT_FAILURE);
      }
      if (sb.st_size == 0) {
        fprintf(stderr, "File is empty\n");
        close(fd_);
        exit(EXIT_FAILURE);
      }

      if (sb.st_size < (long)p_size) {
        fprintf(stderr, "File size is smaller than requested size\n");
        close(fd_);
        exit(EXIT_FAILURE);
      }
    }

    static size_t GetPageAlignedSize(size_t size) { return (size + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1); }
  };
} // namespace sp

#endif // MMF_HPP