#ifndef ZMQSTREAM_H
#define ZMQSTREAM_H

#include <node.h>

namespace zmqstream {
  //
  // ## ScopedContext
  //
  // At the moment, there's a 1:1 relationship between Node processes and ZMQ contexts. This scope-based wrapper
  // is used to ensure that global context is managed properly over the life of that process.
  //
  class ScopedContext {
    public:
      void *context;

      ScopedContext();
      ~ScopedContext();
  };

  //
  // ## Socket
  //
  // Much like the native `net` module, a ZMQStream socket (perhaps obviously) is really just a Duplex stream that
  // you can `connect`, `bind`, etc. just like a native ZMQ socket.
  //
  class Socket : public node::ObjectWrap {
    public:
      static v8::Persistent<v8::FunctionTemplate> constructor;
      static void Init(v8::Handle<v8::Object> target);

      virtual ~Socket();

    protected:
      Socket(int type);

      //
      // ## Socket(type)
      //
      // Creates a new **type** ZMQ socket.
      //
      static v8::Handle<v8::Value> New(const v8::Arguments& args);

      //
      // ## Close `Close()`
      //
      // Closes the underlying ZMQ socket. _The stream should no longer be used!_
      //
      static v8::Handle<v8::Value> Close(const v8::Arguments& args);

      //
      // ## Read `Read(size)`
      //
      // Consumes a minimum of **size** messages of data from the ZMQ socket. If **size** is undefined, the entire
      // queue will be read and returned.
      //
      // If there is no data to consume, or if there are fewer bytes in the internal buffer than the size argument,
      // then null is returned, and a future 'readable' event will be emitted when more is available.
      //
      // Calling stream.read(0) is a no-op with no internal side effects.
      //
      // Returns an Array of Messages, which are in turn Arrays of Frames as Node Buffers.
      //
      // NOTE: To reiterate, this Read returns a different format than the builtin Duplex, which is a single Buffer or
      // String. Additionally, there is no encoding support.
      //
      static v8::Handle<v8::Value> Read(const v8::Arguments& args);

      //
      // ## Write `Write(message)`
      //
      // Writes **message** to the ZMQ socket to be transmitted over the wire at some time in the future.
      //
      // Returns true if **message** was queued successfully, or false if the buffer is full (see ZMQ_DONTWAIT/EAGAIN).
      //
      // NOTE: Unlike the builtin Duplex class, a return value of `false` indicates the write was _unsuccessful_, and
      // will need to be tried again.
      //
      static v8::Handle<v8::Value> Write(const v8::Arguments& args);

      // static v8::Handle<v8::Value> Connect(const v8::Arguments& args);
      // static v8::Handle<v8::Value> Disconnect(const v8::Arguments& args);
      // static v8::Handle<v8::Value> Bind(const v8::Arguments& args);
      // static v8::Handle<v8::Value> Unbind(const v8::Arguments& args);

      void *socket;
  };

  //
  // ## Helpers
  //
  #define ZMQ_DEFINE_CONSTANT(target, name, constant)                       \
    (target)->Set(v8::String::NewSymbol(name),                              \
                  v8::Integer::New(constant),                               \
                  static_cast<v8::PropertyAttribute>(                       \
                      v8::ReadOnly|v8::DontDelete))
}

#endif
