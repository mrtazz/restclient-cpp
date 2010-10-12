require 'rubygems'
require 'sinatra'
get '/?' do
  "GET succesful."
end

post '/?' do
  params
end

put '/?' do
  params
end

delete '/?' do
  "DELETE succesful."
end
