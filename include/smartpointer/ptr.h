/*
 *****************************************************************************
 * Copyright (C) 2012 - 2014 Institute of Information Technology,
 * Alpen-Adria-Universität Klagenfurt
 *
 * Created on: May 20, 2014
 * Authors: Benjamin Rainer <benjamin.rainer@itec.aau.at>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef _PTR_H_
#define _PTR_H_

// TODO: make protected pointer really protected....

template<class T> class protected_ptr{
private:
    
	T *pn;
    
public:
    
	protected_ptr() : pn(0){
        
	};
    
	template<class Y>  explicit protected_ptr(Y *in){
		
        pn = (T *)in;
	};
    
	T *operator ->() {
		return pn;
	};
    
	T *get_pointer(){
        
		return pn;
	};
    
	protected_ptr& operator=(protected_ptr &p){
        
		this->pn = p.get_pointer();
		return *this;
	};
    
	protected_ptr& operator=(T *p){
        
		this->pn = p;
		return *this;
	};
    
	T operator*(){
        
		return *this->pn;
	};
};

template<class T> class scoped_ptr
{
private:
	T *pn;

public:
	scoped_ptr() : pn(0) {};
	~scoped_ptr() {
		delete pn;
	}
	template<class Y> explicit scoped_ptr(Y *in)
	{
		pn = (T *) in;
	};
	
	T *operator->()
	{
		return pn;
	};

	T *get_pointer()
	{
		return pn;
	};

	scoped_ptr & operator=(scoped_ptr &p)
	{
		this->pn = p.get_pointer();
		return *this;
	};

	scoped_ptr & operator=(T *p)
	{
		this->pn = p;
		return *this;
	};

	T operator*()
	{
		return *this->pn;
	};
};


template<class T> class ptr
{
private:

	T *pn;

public:

	ptr() : pn(0){

	};

	template<class Y>  explicit ptr(Y *in){	
		
	 pn = (T *)in;	
	};

	T *operator ->() {
		return pn;
	};

	T *get_pointer(){

		return pn;
	};

	ptr & operator=(ptr &p){

		this->pn = p.get_pointer();
		return *this;
	};

	ptr & operator=(T *p){

		this->pn = p;
		return *this;
	};

	T operator*(){

		return *this->pn;
	};
	
};

#endif