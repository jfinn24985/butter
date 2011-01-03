#ifndef BUTTER_CONST_TOKEN_ITERATOR_HPP
#define BUTTER_CONST_TOKEN_ITERATOR_HPP


// Manual includes
#include <string>
#include <stdexcept>
#include <stack>
#include <iterator>
#if QT_VERSION < 300L
#include <qstring.h>
#endif
//
namespace butter {

/**
 * Provide an iterator over the tokens in a string
 * 
 * This class will break a string into tokens on the given separator
 * character.  It will ignore separators that are in quotation marks
 * or escaped.  An error is thrown if the end of the string is reached
 * and no matching closing quote was found.
 */
class const_token_iterator: public std::iterator< std::forward_iterator_tag, std::string >
{
  public:
    typedef char char_type;

    typedef std::string string_type;

    typedef std::string::size_type size_type;

    const_token_iterator()
    : orig_()
    , tmp_()
    , s_(0)
    , e_(0)
    , esc_(0)
    , sep_(0)
    {}
#if QT_VERSION < 300L
    const_token_iterator(QString orig, char_type sep)
: orig_(orig.latin1())
, tmp_()
, s_(0)
, e_(0)
, esc_('\\')
, sep_(sep.latin1())
{ this->increment(); }
#endif
    const_token_iterator(string_type orig, char_type sep)
    : orig_(orig)
    , tmp_()
    , s_(0)
    , e_(0)
    , esc_('\\')
    , sep_(sep)
    { this->increment(); }
    const_token_iterator(const const_token_iterator & orig)
    : orig_(orig.orig_)
    , tmp_(orig.tmp_)
    , s_(orig.s_)
    , e_(orig.e_)
    , esc_(orig.esc_)
    , sep_(orig.sep_)
    {}
    void swap(const_token_iterator & rhs)
    {
      std::swap(const_cast< string_type& >(this->orig_)
         		, const_cast< string_type& >(rhs.orig_));
      std::swap(this->tmp_, rhs.tmp_);
      std::swap(this->s_, rhs.s_);
      std::swap(this->e_, rhs.e_);
      std::swap(const_cast< char_type& >(this->esc_), const_cast< char_type& >(rhs.esc_));
      std::swap(const_cast< char_type& >(this->sep_), const_cast< char_type& >(rhs.sep_));
    }
    const_token_iterator & operator =(const_token_iterator orig)
    {
      this->swap (orig);
      return *this;
    }
    const_token_iterator & operator ++()
    {
      this->increment();
      return *this;
    }
    const_token_iterator operator ++(int )
    {
      const_token_iterator tmp(*this);
      this->increment();
      return tmp;
    }
    friend bool operator ==(const const_token_iterator & lhs, const const_token_iterator & rhs)
    {
         return lhs.at_end ()
    		? rhs.at_end ()
    		: rhs.at_end ()
    			? false
    			: (lhs.s_ == rhs.s_) & (lhs.e_ == rhs.e_)
    				? lhs.orig_ == rhs.orig_
    				: false;
    }
    friend inline bool operator !=(const const_token_iterator & lhs, const const_token_iterator & rhs)
    {
      return !(lhs == rhs);
    }
    const string_type* const operator ->() const
    {
      return &(this->operator*());
    }
    const string_type& operator *() const
    {
      if (this->tmp_.empty ())
      {
        if (this->s_ != this->e_)
        {
          this->tmp_ = this->orig_.substr(this->s_, this->e_ - this->s_);
        }
      }
      return this->tmp_;
    }

  private:
    bool at_end() const
    {
          return this->orig_.empty ()
                ? true
                : this->s_ >= this->orig_.size (); 
    }
    inline void increment();
    static bool is_quote(const char_type c)
    {
      return (c == '\'') | (c == '"') | (c == '`');
    } 

    /**
     * Reset iterator to the beginning of the contained string
     */
    void reset()
    {
      this->s_ = 0; this->e_ = 0;
    }

    const string_type orig_;

    mutable string_type tmp_;

    size_type s_;

    size_type e_;

    const char_type esc_;

    const char_type sep_;


};

inline void const_token_iterator::increment() 
{
// Bouml preserved body begin 0003B429
if (! this->tmp_.empty ()) { this->tmp_.clear (); }
if (this->e_ >= this->orig_.size ()) { this->s_ = this->e_; return; }
if (this->sep_ == this->orig_[this->e_]) { this->e_++; }
this->s_ = this->e_;
std::stack< char_type > quotes_;
while (this->e_ < this->orig_.size ())
{
  const char_type c_ (this->orig_[this->e_]);
  if (this->esc_ == c_)
  {
    this->e_++;
  }
  else if (this->is_quote(c_))
  {
    if (quotes_.empty () || quotes_.top () != c_)
    {
      quotes_.push (c_);
    }
    else
    {
      quotes_.pop ();
    }
  }
  else if (this->sep_ == c_)
  {
    if (quotes_.empty ()) return;
  }
  this->e_++;
}
if (! quotes_.empty ())
{
  throw std::runtime_error ("Unmatched quotes in string: " + this->orig_);
}
return;
// Bouml preserved body end 0003B429

}


} // namespace butter

namespace std {
	inline void swap(butter::const_token_iterator & lhs, butter::const_token_iterator & rhs)
	{ lhs.swap (rhs); }
}
#endif
