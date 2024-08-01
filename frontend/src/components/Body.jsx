import React from 'react'
import Mockup from '../assets/figmaMockup.PNG'
import FilterCheckBox from './FilterCheckBox.jsx'

const Body = () => {
  return (
    <div className='bg-[#393939] h-[95vh] '> 

        {/* FIXME: */}
        {/* Filter CheckBoxes */}
        <div className='flex justify-start p-6'>
            <div className='flex flex-col text-white'>
                <FilterCheckBox /> 
            </div>
        </div>  

        {/* TODO: */} 
        {/* where the main tree ui will be  */}
        <div className='flex justify-center'>
            <img src={Mockup} className='w-3/4' alt='mockup' />
        </div>
    </div>
        
  )
}

export default Body