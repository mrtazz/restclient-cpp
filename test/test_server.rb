require 'rubygems'
require 'sinatra'
get '/?' do
  "GET succesful."
end

post '/?' do
  request.body()
end

put '/?' do
  request.body()
end

delete '/?' do
  "DELETE succesful."
end
